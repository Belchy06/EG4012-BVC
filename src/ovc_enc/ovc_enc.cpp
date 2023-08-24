#include "ovc_enc.h"

#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>

#include "ovc_common/util/util.h"

ovc_encoder::ovc_encoder()
	: send_vps(true)
{
}

ovc_enc_result ovc_encoder::init(ovc_enc_config* in_config)
{
	// Validate config
	if (in_config->width == 0)
	{
		return OVC_ENC_INVALID_DIMENSIONS;
	}

	if (in_config->height == 0)
	{
		return OVC_ENC_INVALID_DIMENSIONS;
	}

	if (in_config->format == ovc_chroma_format::OVC_CHROMA_FORMAT_UNDEFINED)
	{
		return OVC_ENC_INVALID_FORMAT;
	}

	if ((in_config->num_levels > 1 || in_config->num_streams_exp > 0) && in_config->partition_type == OVC_PARTITION_SKIP)
	{
		return OVC_ENC_INVALID_PARAM;
	}

	// Calculate levels from streams or vice-versa. If user has specified both, ensure that the values are compatible
	if ((in_config->num_levels != -1) && (in_config->num_streams_exp == -1))
	{
		in_config->num_streams_exp = (int)(in_config->width * in_config->height / pow(4, in_config->num_levels));
	}
	else if ((in_config->num_streams_exp != -1) && (in_config->num_levels == -1))
	{
		in_config->num_levels = (int)(log2(in_config->width * in_config->height / pow(4, in_config->num_streams_exp)) / log2(4));
	}
	else if ((in_config->num_levels != -1) && (in_config->num_streams_exp != -1))
	{
		if (pow(4, in_config->num_streams_exp) > in_config->width * in_config->height / pow(4, in_config->num_levels))
		{
			return OVC_ENC_INVALID_PARAM;
		}
	}

	config = *in_config;

	wavelet_decomposer = ovc_wavelet_decomposer_factory::create_wavelet_decomposer(in_config->wavelet_family, in_config->wavelet_config);
	partitioner = ovc_partitioner_factory::create_partitioner(in_config->partition_type);
	spiht_encoder = ovc_spiht_encoder_factory::create_spiht_encoder(in_config->spiht);
	entropy_coder = ovc_entropy_encoder_factory::create_entropy_encoder(in_config->entropy_coder);

	return ovc_enc_result::OVC_ENC_OK;
}

ovc_enc_result ovc_encoder::encode(ovc_picture* in_picture, ovc_nal** out_nal_units, size_t* out_num_nal_units)
{
	output_nals.clear();

	// VPS is 1 per video (or picture if configured for such)
	// VPS NAL
	construct_and_output_vps();

	// TODO (belchy06): Parallelize
	for (uint8_t component = 0; component < (uint8_t)(config.format == OVC_CHROMA_FORMAT_MONOCHROME ? 1 : 3); component++)
	{
		size_t				plane_width = (component == 0 ? config.width : util::scale_x(config.width, config.format));
		size_t				plane_height = (component == 0 ? config.height : util::scale_y(config.height, config.format));
		matrix<double>		plane_matrix(plane_height, plane_width);
		std::vector<double> plane_data;
		for (size_t x = 0; x < plane_width; x++)
		{
			for (size_t y = 0; y < plane_height; y++)
			{
				uint8_t* plane = in_picture->planes[component].data;
				plane_data.push_back((double)plane[x + y * plane_width]);
			}
		}

		plane_matrix.set_data(plane_data);

		ovc_wavelet_decomposition_2d<double> decomposition = wavelet_decomposer->decompose(plane_matrix, config.num_levels);
		matrix<double>						 decomp_matrix = decomposition.to_matrix();

		std::vector<matrix<double>> partitions = partitioner->partition(decomp_matrix, config.num_levels, (size_t)pow(4, config.num_streams_exp));
		// TODO (belchy06): Parallelize
		for (uint16_t i = 0; i < partitions.size(); i++)
		{
			matrix<double>& partition = partitions[i];
			spiht_encoder->encode(partition, { .bpp = config.bits_per_pixel, .num_levels = (size_t)config.num_levels });
			uint8_t* spiht_bitstream = new uint8_t();
			size_t	 spiht_byte_length = 0;
			int		 spiht_step_size = 0;
			spiht_encoder->flush(&spiht_bitstream, &spiht_byte_length, &spiht_step_size);

			entropy_coder->encode(spiht_bitstream, spiht_byte_length);
			uint8_t* entropy_bitstream = new uint8_t();
			size_t	 entropy_byte_length = 0;
			entropy_coder->flush(&entropy_bitstream, &entropy_byte_length);

			std::vector<uint8_t> bytes;
			for (size_t j = 0; j < entropy_byte_length; j++)
			{
				bytes.push_back(entropy_bitstream[j]);
			}

			// PPS is one per partition
			// PPS NAL
			construct_and_output_pps(component, i, partition.get_num_columns(), partition.get_num_rows(), spiht_byte_length, spiht_step_size);

			// PARTITION NAL
			/* NAL HEADER (2 BYTES) */
			/*
			 +---------------+---------------+
			 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
			 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			 |     START     | Z |     T     |
			 +---------------+---------------|

			START = 0x0       (8)
			Z     = Zero bits (2)
			T     = Nal Type  (8)
			*/
			std::vector<uint8_t> nal_header;
			uint8_t				 nal_header_byte;
			nal_header_byte = 0;
			nal_header_byte |= 0x0; // START
			nal_header.push_back(nal_header_byte);

			nal_header_byte = 0;
			// clang-format off
            nal_header_byte |= (0                      << 6) & 0b11000000; // Z
			nal_header_byte |= (OVC_NAL_TYPE_PARTITION << 0) & 0b00111111; // T
			nal_header.push_back(nal_header_byte);
			// clang-format on

			/* PARTITION FORMAT (VARIABLE BYTES) (0x2) */
			/*
			+---------------+---------------+---------------+---------------+
			|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
			+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			| C |      PARTITION_ID         |       PARTITION DATA ...      |
			+---------------+---------------+                               +
			|                                                               |
			|                               +---------------+---------------+
			|                               :
			+---------------+---------------|
			*/

			std::vector<uint8_t> partition_header;
			uint8_t				 partition_header_byte;
			partition_header_byte = 0;
			// clang-format off
			partition_header_byte |= (component    << 6) & 0b11000000;
			partition_header_byte |= (i            >> 8) & 0b00111111;
			partition_header.push_back(partition_header_byte);
			// clang-format on

			partition_header_byte = 0;
			// clang-format off
			partition_header_byte |= (i >> 0) & 0b11111111;
			partition_header.push_back(partition_header_byte);
			// clang-format on

			std::vector<uint8_t> nal_bytes;
			nal_bytes.reserve(nal_header.size() + partition_header.size() + bytes.size());
			nal_bytes.insert(nal_bytes.end(), nal_header.begin(), nal_header.end());
			nal_bytes.insert(nal_bytes.end(), partition_header.begin(), partition_header.end());
			nal_bytes.insert(nal_bytes.end(), bytes.begin(), bytes.end());

			ovc_nal nal;
			nal.bytes = new uint8_t[nal_bytes.size()]{ 0 };
			memcpy(nal.bytes, nal_bytes.data(), nal_bytes.size());
			nal.size = nal_bytes.size();
			output_nals.push_back(nal);
		}
	}

	*out_nal_units = &output_nals[0];
	*out_num_nal_units = static_cast<int>(output_nals.size());

	return ovc_enc_result::OVC_ENC_OK;
}

void ovc_encoder::construct_and_output_vps()
{
	if (send_vps)
	{
		/* NAL HEADER (2 BYTES) */
		/*
		 +---------------+---------------+
		 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |     START     | Z |     T     |
		 +---------------+---------------+

		START = 0x0       (8)
		Z     = Zero bits (2)
		T     = Nal Type  (8)
		*/
		std::vector<uint8_t> header;
		uint8_t				 header_byte;
		header_byte = 0;
		header_byte |= 0x0; // START
		header.push_back(header_byte);

		header_byte = 0;
		// clang-format off
        header_byte |= (0                << 6) & 0b11000000; // Z
		header_byte |= (OVC_NAL_TYPE_VPS << 0) & 0b00111111; // T
		header.push_back(header_byte);
		// clang-format on

		/* VPS FORMAT (10 BYTES) (0x0) */
		/*
		 +---------------+---------------+---------------+---------------+
		 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 | W F |   W C   | P | S | E |FMT|              N L              |
		 +---------------+---------------+---------------+---------------+
		 |              N S              |              BPP              |
		 +---------------+---------------+---------------+---------------+
		 |           BPP (CONT)          |
		 +---------------+---------------+

		 W F = Wavelet Family (3)
		 W C = Wavelet Config (5)
		 P   = Partitoner     (2)
		 S   = Spiht          (2)
		 E   = Entropy Coder  (2)
		 FMT = Format         (2)
		 N L = # Levels		  (16)
		 N S = # Streams	  (16)
		 BPP = Bits Per Pixel (32)
		*/
		std::vector<uint8_t> vps;
		uint8_t				 vps_byte;
		vps_byte = 0;
		// clang-format off
		vps_byte |= (config.wavelet_family       << 5) & 0b11100000;
		vps_byte |= (config.wavelet_config.value << 0) & 0b00011111;
		vps.push_back(vps_byte);
		// clang-format on

		vps_byte = 0;
		// clang-format off
		vps_byte |= (config.partition_type << 6) & 0b11000000;
		vps_byte |= (config.spiht          << 4) & 0b00110000;
		vps_byte |= (config.entropy_coder  << 2) & 0b00001100;
		vps_byte |= (config.format         << 0) & 0b00000011;
		vps.push_back(vps_byte);
		// clang-format on

		vps.push_back((uint8_t)(config.num_levels >> 8));
		vps.push_back((uint8_t)(config.num_levels >> 0));

		vps.push_back((uint8_t)((size_t)pow(4, config.num_streams_exp) >> 8));
		vps.push_back((uint8_t)((size_t)pow(4, config.num_streams_exp) >> 0));

		uint8_t* bpp_arr = reinterpret_cast<uint8_t*>(&config.bits_per_pixel);
		vps.push_back(bpp_arr[3]);
		vps.push_back(bpp_arr[2]);
		vps.push_back(bpp_arr[1]);
		vps.push_back(bpp_arr[0]);

		std::vector<uint8_t> nal_bytes;
		nal_bytes.reserve(header.size() + vps.size());
		nal_bytes.insert(nal_bytes.end(), header.begin(), header.end());
		nal_bytes.insert(nal_bytes.end(), vps.begin(), vps.end());

		ovc_nal nal;
		nal.bytes = new uint8_t[nal_bytes.size()]{ 0 };
		memcpy(nal.bytes, nal_bytes.data(), nal_bytes.size());
		nal.size = nal_bytes.size();
		output_nals.push_back(nal);
	}

	send_vps &= config.repeat_vps;
}

void ovc_encoder::construct_and_output_pps(uint8_t in_component, uint16_t in_partition_id, size_t in_width, size_t in_height, size_t in_byte_length, int in_spiht_step_size)
{
	/* NAL HEADER (2 BYTES) */
	/*
	 +---------------+---------------+
	 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
	 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 |     START     | Z |     T     |
	 +---------------+---------------+

	START = 0x0       (8)
	Z     = Zero bits (2)
	T     = Nal Type  (8)
	*/
	std::vector<uint8_t> header;
	uint8_t				 header_byte;
	header_byte = 0;
	header_byte |= 0x0; // START
	header.push_back(header_byte);

	header_byte = 0;
	// clang-format off
    header_byte |= (0                << 6) & 0b11000000; // Z
	header_byte |= (OVC_NAL_TYPE_PPS << 0) & 0b00111111; // T
	header.push_back(header_byte);
	// clang-format on

	/* PPS FORMAT (18 BYTES) (0x1) */
	/*
	 +---------------+---------------+---------------+---------------+
	 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
	 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 |xxxxxxxxxxxxxxx|xxxxxxxxxxxxxxx| C |      PARTITION_ID         |
	 +---------------+---------------+---------------+---------------+
	 |                             WIDTH                             |
	 +---------------+---------------+---------------+---------------+
	 |                             HEIGHT                            |
	 +---------------+---------------+---------------+---------------+
	 |                             NUM_SYM                           |
	 +---------------+---------------+---------------+---------------+
	 |                             STEP                              |
	 +---------------+---------------+---------------+---------------+

	 C = Component (2)
	*/
	std::vector<uint8_t> pps;
	uint8_t				 pps_byte;
	pps_byte = 0;
	// clang-format off
	pps_byte |= (in_component    << 6) & 0b11000000;
	pps_byte |= (in_partition_id >> 8) & 0b00111111;
	pps.push_back(pps_byte);
	// clang-format on

	pps_byte = 0;
	// clang-format off
	pps_byte |= (in_partition_id >> 0) & 0b11111111;
	pps.push_back(pps_byte);
	// clang-format on

	pps.push_back((uint8_t)(in_width >> 24));
	pps.push_back((uint8_t)(in_width >> 16));
	pps.push_back((uint8_t)(in_width >> 8));
	pps.push_back((uint8_t)(in_width >> 0));

	pps.push_back((uint8_t)(in_height >> 24));
	pps.push_back((uint8_t)(in_height >> 16));
	pps.push_back((uint8_t)(in_height >> 8));
	pps.push_back((uint8_t)(in_height >> 0));

	pps.push_back((uint8_t)(in_byte_length >> 24));
	pps.push_back((uint8_t)(in_byte_length >> 16));
	pps.push_back((uint8_t)(in_byte_length >> 8));
	pps.push_back((uint8_t)(in_byte_length >> 0));

	pps.push_back((uint8_t)(in_spiht_step_size >> 24));
	pps.push_back((uint8_t)(in_spiht_step_size >> 16));
	pps.push_back((uint8_t)(in_spiht_step_size >> 8));
	pps.push_back((uint8_t)(in_spiht_step_size >> 0));

	std::vector<uint8_t> nal_bytes;
	nal_bytes.reserve(header.size() + pps.size());
	nal_bytes.insert(nal_bytes.end(), header.begin(), header.end());
	nal_bytes.insert(nal_bytes.end(), pps.begin(), pps.end());

	ovc_nal nal;
	nal.bytes = new uint8_t[nal_bytes.size()]{ 0 };
	memcpy(nal.bytes, nal_bytes.data(), nal_bytes.size());
	nal.size = nal_bytes.size();
	output_nals.push_back(nal);
}