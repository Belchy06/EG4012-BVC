#include <cmath>
#include <cstring>

#include "ovc_common/log.h"
#include "ovc_dec/ovc_dec.h"

#define LogDecode "LogDecode"

ovc_decoder::ovc_decoder()
	: picture_ready(false)
	, initialised(false)
{
}

ovc_dec_result ovc_decoder::init(ovc_dec_config* in_config)
{
	ppss[0] = std::map<size_t, ovc_pps>();
	ppss[1] = std::map<size_t, ovc_pps>();
	ppss[2] = std::map<size_t, ovc_pps>();

	partitions[0] = std::map<size_t, matrix<double>>();
	partitions[1] = std::map<size_t, matrix<double>>();
	partitions[2] = std::map<size_t, matrix<double>>();

	ovc_logging::verbosity = in_config->log_verbosity;

	config = *in_config;

	initialised = true;

	return OVC_DEC_OK;
}

ovc_dec_result ovc_decoder::decode_nal(const ovc_nal* in_nal_unit)
{
	if (!initialised)
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Dropping NAL. Decoder uninitialised");
		return OVC_DEC_UNINITIALISED;
	}

	uint8_t* nal_bytes = new uint8_t[in_nal_unit->size]{ 0 };
	size_t	 nal_size = in_nal_unit->size;
	memcpy(nal_bytes, in_nal_unit->bytes, in_nal_unit->size);

	if (nal_size < 2)
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Dropping NAL due to malformed header (size < 2 (bytes))");
		return OVC_DEC_MALFORMED_NAL_HEADER;
	}

	/* NAL HEADER (2 BYTES) */
	/*
	 +---------------+---------------+
	 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
	 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 |     START     | Z |     T     |
	 +---------------+---------------+

	START = 0x1       (8)
	Z     = Zero bits (2)
	T     = Nal Type  (6)
	*/
	// clang-format off
	uint8_t		 start_byte = nal_bytes[0];
    uint8_t		 type_byte  = nal_bytes[1];
	uint8_t		 zero_bits  =          (type_byte & 0b11000000) >> 6;
	ovc_nal_type type = (ovc_nal_type)((type_byte & 0b00111111) >> 0);
    nal_bytes += 2;
    nal_size -= 2;
	// clang-format on
	if ((start_byte & zero_bits) != 0)
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Dropping NAL due to malformed header (start & res) != 0");
		return OVC_DEC_MALFORMED_NAL_HEADER;
	}

	switch (type)
	{
		case OVC_NAL_TYPE_VPS:
			return handle_vps(nal_bytes, nal_size);
		case OVC_NAL_TYPE_PPS:
			return handle_pps(nal_bytes, nal_size);
		case OVC_NAL_TYPE_PARTITION:
			return handle_partition(nal_bytes, nal_size);
		default:
			LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unknown NAL type");
			return OVC_DEC_MALFORMED_NAL_HEADER;
	}
}

ovc_dec_result ovc_decoder::get_picture(ovc_picture* out_picture)
{
	if (!picture_ready)
	{
		// We haven't received enough NALs to produce a full picture
		return OVC_DEC_NO_PICTURE;
	}

	// TODO (belchy06): Parse headers
	*out_picture = std::move(picture);
	out_picture->framerate = 29.97f;

	partitions.clear();
	partitions[0] = std::map<size_t, matrix<double>>();
	partitions[1] = std::map<size_t, matrix<double>>();
	partitions[2] = std::map<size_t, matrix<double>>();

	picture_ready = false;
	picture = ovc_picture();

	return OVC_DEC_OK;
}

ovc_dec_result ovc_decoder::handle_vps(uint8_t* in_bytes, size_t in_size)
{
	/* VPS FORMAT (10 BYTES) (0x0) */
	/*
	 +---------------+---------------+---------------+---------------+
	 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
	 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 | W F |   W C   | P | S | E |FMT|              N L              |
	 +---------------+---------------+---------------+---------------+
	 |              N P              |              BPP              |
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
	 N P = # Partitions	  (16)
	 BPP = Bits Per Pixel (32)
	*/
	if (in_size < 10)
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode VPS (size < 10 (bytes))");
		return OVC_DEC_MALFORMED_NAL_BODY;
	}

	// Parse values for vps
	size_t	byte_idx = 0;
	uint8_t vps_byte = 0;
	// clang-format off
	vps_byte = in_bytes[byte_idx++];
	vps.wavelet_family =  (ovc_wavelet_family)((vps_byte & 0b11100000) >> 5);
	vps.wavelet_config =  { .value = (uint8_t)((vps_byte & 0b00011111) >> 0) };
	// clang-format on

	// clang-format off
	vps_byte = in_bytes[byte_idx++];
	vps.partition_type = (ovc_partition)    ((vps_byte & 0b11000000) >> 6);
	vps.spiht          = (ovc_spiht)        ((vps_byte & 0b00110000) >> 4);
	vps.entropy_coder  = (ovc_entropy_coder)((vps_byte & 0b00001100) >> 2);
    vps.format         = (ovc_chroma_format)((vps_byte & 0b00000011) >> 0);
	// clang-format on

	vps.num_levels = 0;
	vps.num_levels |= in_bytes[byte_idx++] << 8;
	vps.num_levels |= in_bytes[byte_idx++] << 0;

	vps.num_partitions = 0;
	vps.num_partitions |= in_bytes[byte_idx++] << 8;
	vps.num_partitions |= in_bytes[byte_idx++] << 0;

	union
	{
		int	  bpp_int = 0;
		float bpp_float;
	} bits_per_pixel;

	bits_per_pixel.bpp_int |= in_bytes[byte_idx++] << 24;
	bits_per_pixel.bpp_int |= in_bytes[byte_idx++] << 16;
	bits_per_pixel.bpp_int |= in_bytes[byte_idx++] << 8;
	bits_per_pixel.bpp_int |= in_bytes[byte_idx++] << 0;
	vps.bits_per_pixel = bits_per_pixel.bpp_float;

	vps.is_set = true;

	// Intialise components based on specified vps
	wavelet_recomposer = ovc_wavelet_recomposer_factory::create_wavelet_recomposer(vps.wavelet_family, vps.wavelet_config);
	departitioner = ovc_departitioner_factory::create_departitioner(vps.partition_type);
	spiht_decoder = ovc_spiht_decoder_factory::create_spiht_decoder(vps.spiht);
	entropy_decoder = ovc_entropy_decoder_factory::create_entropy_decoder(vps.entropy_coder);

	return OVC_DEC_OK;
}

ovc_dec_result ovc_decoder::handle_pps(uint8_t* in_bytes, size_t in_size)
{
	/* PPS FORMAT (14 BYTES) (0x1) */
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
	 |                             STEP                              |
	 +---------------+---------------+---------------+---------------+

	 C = Component (2)
	*/
	if (in_size < 14)
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode PPS (size < 18 (bytes))");
		return OVC_DEC_MALFORMED_NAL_BODY;
	}

	// Extract values for pps
	size_t	byte_idx = 1;
	ovc_pps pps = ovc_pps();
	pps.component = 0;
	pps.component |= (in_bytes[0] & 0b11000000) >> 6;

	pps.partition = 0;
	pps.partition |= (in_bytes[0] & 0b00111111) << 8;
	pps.partition |= in_bytes[byte_idx++] << 0;

	pps.width = 0;
	pps.width |= in_bytes[byte_idx++] << 24;
	pps.width |= in_bytes[byte_idx++] << 16;
	pps.width |= in_bytes[byte_idx++] << 8;
	pps.width |= in_bytes[byte_idx++] << 0;

	pps.height = 0;
	pps.height |= in_bytes[byte_idx++] << 24;
	pps.height |= in_bytes[byte_idx++] << 16;
	pps.height |= in_bytes[byte_idx++] << 8;
	pps.height |= in_bytes[byte_idx++] << 0;

	pps.step = 0;
	pps.step |= in_bytes[byte_idx++] << 24;
	pps.step |= in_bytes[byte_idx++] << 16;
	pps.step |= in_bytes[byte_idx++] << 8;
	pps.step |= in_bytes[byte_idx++] << 0;

	pps.is_set = true;

	ppss[pps.component][pps.partition] = pps;

	return OVC_DEC_OK;
}

ovc_dec_result ovc_decoder::handle_partition(uint8_t* in_bytes, size_t in_size)
{
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
	if (in_size < 2)
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode partition (size < 2 (bytes))");
		return OVC_DEC_MALFORMED_NAL_BODY;
	}

	if (!vps.is_set)
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode partition (No VPS received)");
		return OVC_DEC_MISSING_VPS;
	}

	size_t	byte_idx = 0;
	uint8_t partition_byte = 0;
	// clang-format off
	partition_byte = in_bytes[byte_idx++];
	uint8_t component = 0;
    component |= (partition_byte & 0b11000000) >> 6;

    uint16_t partition_id = 0;
    partition_id |= (partition_byte & 0b00111111) << 8;
    partition_id |=        (in_bytes[byte_idx++]) << 0;
	// clang-format on

	if (!ppss[component].contains(partition_id))
	{
		LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode partition (No PPS received)");
		LOG(LogDecode, OVC_VERBOSITY_DETAILS, "Component: {}", (component == 0 ? "Y" : (component == 1 ? "U" : "V")));
		LOG(LogDecode, OVC_VERBOSITY_DETAILS, "Partition: {}", partition_id);
		return OVC_DEC_MISSING_PPS;
	}

	// Update data to account for the two start bytes read
	in_bytes += 2;
	in_size -= 2;

	ovc_pps pps = ppss[component][partition_id];

	// Decode
	//                               num_bytes            num_symbols (bits)
	entropy_decoder->decode(in_bytes, in_size, (size_t)(ceil(pps.width * pps.height * vps.bits_per_pixel)));
	uint8_t* entropy_decoded_bytes = new uint8_t();
	size_t	 entropy_decoded_size = 0;
	entropy_decoder->flush(&entropy_decoded_bytes, &entropy_decoded_size);

	spiht_decoder->decode(entropy_decoded_bytes, entropy_decoded_size, pps.width, pps.height, { .bpp = vps.bits_per_pixel, .num_levels = (size_t)vps.num_levels, .step = pps.step });
	matrix<double> partition = matrix<double>(pps.height, pps.width);
	spiht_decoder->flush(partition);

	std::map<size_t, matrix<double>>& plane_partitions = partitions[component];
	plane_partitions[partition_id] = partition;

	if (plane_partitions.size() == (size_t)vps.num_partitions)
	{
		std::vector<matrix<double>> parts;
		for (size_t i = 0; i < (size_t)vps.num_partitions; i++)
		{
			parts.push_back(plane_partitions[i]);
		}
		matrix<double> full = departitioner->departition(parts, vps.num_levels);

		ovc_wavelet_decomposition_2d<double> decomposition = ovc_wavelet_decomposition_2d<double>::from_matrix(full, vps.num_levels);
		matrix<double>						 image = wavelet_recomposer->recompose(decomposition, full.size());

		std::vector<uint8_t> plane_data;
		for (size_t y = 0; y < image.get_num_rows(); y++)
		{
			for (size_t x = 0; x < image.get_num_columns(); x++)
			{
				double pixel = trunc(image(y, x));
				plane_data.push_back((uint8_t)pixel);
			}
		}

		ovc_plane plane;
		plane.data = new uint8_t[image.get_num_rows() * image.get_num_columns()]{ 0 };

		plane.height = image.get_num_rows();
		plane.width = image.get_num_columns();
		plane.bit_depth = 8;
		memcpy(plane.data, plane_data.data(), plane_data.size());

		picture.planes[component] = plane;
		picture.format = vps.format;
	}

	if (vps.format == OVC_CHROMA_FORMAT_MONOCHROME)
	{
		picture_ready = picture.planes[0].width != 0;
	}
	else
	{
		picture_ready = picture.planes[0].width != 0 && picture.planes[1].width != 0 && picture.planes[2].width != 0;
	}

	return ovc_dec_result::OVC_DEC_OK;
}

#undef LogDecode