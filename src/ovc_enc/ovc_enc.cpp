#include "ovc_enc.h"

#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>

#include "ovc_common/util/util.h"

ovc_encoder::ovc_encoder()
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

	// TODO (belchy06): Parallelize
	for (size_t component = 0; component < (size_t)(config.format == OVC_CHROMA_FORMAT_MONOCHROME ? 1 : 3); component++)
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

		std::vector<matrix<double>> streams = partitioner->partition(decomp_matrix, config.num_levels, (size_t)pow(4, config.num_streams_exp));
		// TODO (belchy06): Parallelize
		size_t streamId = 0;
		for (matrix<double> stream : streams)
		{
			spiht_encoder->encode(stream, { .bpp = config.bits_per_pixel, .num_levels = (size_t)config.num_levels });
			uint8_t* spiht_bitstream = new uint8_t();
			size_t	 spiht_byte_length = 0;
			int		 spiht_step_size = 0;
			spiht_encoder->flush(&spiht_bitstream, &spiht_byte_length, &spiht_step_size);

			entropy_coder->encode(spiht_bitstream, spiht_byte_length);
			uint8_t* entropy_bitstream = new uint8_t();
			size_t	 entropy_byte_length = 0;
			entropy_coder->flush(&entropy_bitstream, &entropy_byte_length);

			std::vector<uint8_t> bytes;
			for (size_t i = 0; i < entropy_byte_length; i++)
			{
				bytes.push_back(entropy_bitstream[i]);
			}

			/*
			 +---------------+---------------+---------------+---------------+
			 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
			 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			 | W F |   W C   | P | E |S| C | |FMT|       NUM_LEVELS          |
			 +---------------+---------------+---------------+---------------+
			 |          NUM_STREAMS          |           STREAM_ID           |
			 +---------------+---------------+---------------+---------------+
			 |                             WIDTH                             |
			 +---------------+---------------+---------------+---------------+
			 |                             HEIGHT                            |
			 +---------------+---------------+---------------+---------------+
			 |                             NUM_SYM                           |
			 +---------------+---------------+---------------+---------------+
			 |                             BPP                               |
			 +---------------+---------------+---------------+---------------+
			 |                             STEP                              |
			 +---------------+---------------+---------------+---------------+
			*/
			std::vector<uint8_t> header;
			uint8_t				 coder_config;
			coder_config = 0;
			coder_config |= (config.wavelet_family << 5) & 0b11100000;
			coder_config |= (config.wavelet_config.value << 0) & 0b11111;
			header.push_back(coder_config);

			coder_config = 0;
			// clang-format off
			coder_config |= (config.partition_type << 6) & 0b11000000;
			coder_config |= (config.entropy_coder  << 4) & 0b00110000;
			coder_config |= (config.spiht          << 3) & 0b00001000;
			coder_config |= (component             << 1) & 0b00000110;
			header.push_back(coder_config);
			// clang-format on

			coder_config = 0;
			// clang-format off
			coder_config |= (config.format << 6)     & 0b11000000;
			coder_config |= (config.num_levels >> 8) & 0b00111111;
			header.push_back(coder_config);
			// clang-format on

			header.push_back((uint8_t)(config.num_levels >> 0));

			header.push_back((uint8_t)((size_t)pow(4, config.num_streams_exp) >> 8));
			header.push_back((uint8_t)((size_t)pow(4, config.num_streams_exp) >> 0));

			header.push_back((uint8_t)(streamId >> 8));
			header.push_back((uint8_t)(streamId >> 0));

			header.push_back((uint8_t)(stream.get_num_columns() >> 24));
			header.push_back((uint8_t)(stream.get_num_columns() >> 16));
			header.push_back((uint8_t)(stream.get_num_columns() >> 8));
			header.push_back((uint8_t)(stream.get_num_columns() >> 0));

			header.push_back((uint8_t)(stream.get_num_rows() >> 24));
			header.push_back((uint8_t)(stream.get_num_rows() >> 16));
			header.push_back((uint8_t)(stream.get_num_rows() >> 8));
			header.push_back((uint8_t)(stream.get_num_rows() >> 0));

			header.push_back((uint8_t)(spiht_byte_length >> 24));
			header.push_back((uint8_t)(spiht_byte_length >> 16));
			header.push_back((uint8_t)(spiht_byte_length >> 8));
			header.push_back((uint8_t)(spiht_byte_length >> 0));

			uint8_t* bpp_arr = reinterpret_cast<uint8_t*>(&config.bits_per_pixel);
			header.push_back(bpp_arr[3]);
			header.push_back(bpp_arr[2]);
			header.push_back(bpp_arr[1]);
			header.push_back(bpp_arr[0]);

			header.push_back((uint8_t)(spiht_step_size >> 24));
			header.push_back((uint8_t)(spiht_step_size >> 16));
			header.push_back((uint8_t)(spiht_step_size >> 8));
			header.push_back((uint8_t)(spiht_step_size >> 0));

			// Append bytes to the end of header
			header.reserve(header.size() + bytes.size());
			header.insert(header.end(), bytes.begin(), bytes.end());

			ovc_nal nal;
			nal.bytes = new uint8_t[header.size()]{ 0 };
			memcpy(nal.bytes, header.data(), header.size());
			nal.size = header.size();
			output_nals.push_back(nal);
			streamId++;
		}
	}

	*out_nal_units = &output_nals[0];
	*out_num_nal_units = static_cast<int>(output_nals.size());

	return ovc_enc_result::OVC_ENC_OK;
}

size_t ovc_encoder::get_size_in_bytes(ovc_chroma_format in_format)
{
	size_t picture_samples = 0;
	if (in_format == ovc_chroma_format::OVC_CHROMA_FORMAT_MONOCHROME)
	{
		picture_samples = config.width * config.height;
	}
	else if (in_format == ovc_chroma_format::OVC_CHROMA_FORMAT_420)
	{
		picture_samples = (3 * (config.width * config.height)) >> 1;
	}
	else if (in_format == ovc_chroma_format::OVC_CHROMA_FORMAT_422)
	{
		picture_samples = 2 * config.width * config.height;
	}
	else if (in_format == ovc_chroma_format::OVC_CHROMA_FORMAT_444)
	{
		picture_samples = 3 * config.width * config.height;
	}
	return picture_samples;
}