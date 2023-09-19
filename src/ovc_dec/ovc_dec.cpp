#include <cmath>
#include <cstring>

#include "ovc_dec/ovc_dec.h"

#define LogDecode "LogDecode"

ovc_decoder::ovc_decoder()
	: picture_ready(false)
	, initialised(false)
{
}

ovc_dec_result ovc_decoder::init(ovc_dec_config* in_config)
{
	picture_ready = false;
	picture = ovc_picture();

	planes.clear();
	planes[0] = std::map<size_t, matrix<double>>();
	planes[1] = std::map<size_t, matrix<double>>();
	planes[2] = std::map<size_t, matrix<double>>();

	ovc_logging::verbosity = in_config->log_verbosity;

	config = *in_config;

	initialised = true;

	return OVC_DEC_OK;
}

void ovc_decoder::set_logging_callback(ovc_logging_callback in_callback)
{
	ovc_logging::logging_function = in_callback;
}

ovc_dec_result ovc_decoder::decode_nal(const ovc_nal* in_nal_unit)
{
	if (!initialised)
	{
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Dropping NAL. Decoder uninitialised");
		return OVC_DEC_UNINITIALISED;
	}

	uint8_t* nal_bytes = new uint8_t[in_nal_unit->size]{ 0 };
	size_t	 nal_size = in_nal_unit->size;
	memcpy(nal_bytes, in_nal_unit->bytes, in_nal_unit->size);

	if (nal_size < 2)
	{
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Dropping NAL due to malformed header (size < 2 (bytes))");
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
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Dropping NAL due to malformed header (start & res) != 0");
		return OVC_DEC_MALFORMED_NAL_HEADER;
	}

	switch (type)
	{
		case OVC_NAL_TYPE_VPS:
			return handle_vps(nal_bytes, nal_size);
		case OVC_NAL_TYPE_PARTITION:
			return handle_partition(nal_bytes, nal_size);
		default:
			OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unknown NAL type");
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

	picture_ready = false;

	return OVC_DEC_OK;
}

ovc_dec_result ovc_decoder::flush()
{
	// Reset our picture memory
	picture = ovc_picture();

	for (uint8_t component = 0; component < (uint8_t)(vps.format == OVC_CHROMA_FORMAT_MONOCHROME ? 1 : 3); component++)
	{
		// Get the partitions for this plane
		std::map<size_t, matrix<double>>& partitions = planes[component];
		if (partitions.size() != (size_t)vps.num_partitions)
		{
			// We've missed a partition. Find which partition it is and supplement with a 0 matrix

			// Get all the partition_ids we currently have
			std::vector<size_t> keys;
			for (auto const& partition : partitions)
			{
				keys.push_back(partition.first);
			}

			// Find which partition_ids are missing
			std::vector<size_t> missing_keys;
			for (size_t i = 0; i < vps.num_partitions; i++)
			{
				if (std::find(keys.begin(), keys.end(), i) == keys.end())
				{
					missing_keys.push_back(i);
				}
			}

			// Supplement this missing partition with a 0 matrix
			size_t width = size_t(sqrt(vps.num_partitions) * (component == 0 ? vps.luma_width : vps.chroma_width) / vps.num_partitions);
			size_t height = size_t(sqrt(vps.num_partitions) * (component == 0 ? vps.luma_height : vps.chroma_height) / vps.num_partitions);
			for (size_t i = 0; i < missing_keys.size(); i++)
			{
				size_t		   missing_key = missing_keys[i];
				matrix<double> supplement_partition = matrix<double>(height, width, 0);
				if (config.error_concealment == OVC_ERROR_CONCEALMENT_AVERAGE_RECEIVED)
				{
					for (size_t y = 0; y < supplement_partition.get_num_rows(); y++)
					{
						for (size_t x = 0; x < supplement_partition.get_num_columns(); x++)
						{
							// Average with other partitions
							double sum = 0;
							for (size_t j = 0; j < keys.size(); j++)
							{
								size_t key = keys[j];
								sum += partitions[key](y, x);
							}
							supplement_partition(y, x) = sum / keys.size();
						}
					}
					OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Supplementing c: %d, p: %d with the average of surrounding values", component, missing_key);
				}
				else if (config.error_concealment == OVC_ERROR_CONCEALMENT_AVERAGE_SURROUNDING)
				{
					for (size_t y = 0; y < supplement_partition.get_num_rows(); y++)
					{
						for (size_t x = 0; x < supplement_partition.get_num_columns(); x++)
						{
							// Average with surrounding partitions in + shape
							double sum = 0;
							size_t num_used_partitions = 0;

							// Right neighbour (if we received it)
							if (std::find(keys.begin(), keys.end(), missing_key + 1) != keys.end())
							{
								sum += partitions[missing_key + 1](y, x);
								num_used_partitions++;
							}

							// Left neighbour (if we received it)
							if (std::find(keys.begin(), keys.end(), missing_key - 1) != keys.end())
							{
								sum += partitions[missing_key - 1](y, x);
								num_used_partitions++;
							}

							// Top neighbour (if we received it)
							if (std::find(keys.begin(), keys.end(), missing_key - (size_t)sqrt(vps.num_partitions)) != keys.end())
							{
								sum += partitions[missing_key - (size_t)sqrt(vps.num_partitions)](y, x);
								num_used_partitions++;
							}

							// Bottom neighbour (if we received it)
							if (std::find(keys.begin(), keys.end(), missing_key + (size_t)sqrt(vps.num_partitions)) != keys.end())
							{
								sum += partitions[missing_key + (size_t)sqrt(vps.num_partitions)](y, x);
								num_used_partitions++;
							}

							supplement_partition(y, x) = sum / num_used_partitions;
						}
					}
					OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Supplementing c: %d, p: %d with the average of surrounding values in a + shape", component, missing_key);
				}
				else if (config.error_concealment == OVC_ERROR_CONCEALMENT_SKIP)
				{
					OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Supplementing c: %d, p: %d with a zero matrix", component, missing_key);
				}

				partitions[missing_key] = supplement_partition;
			}
		}

		std::vector<matrix<double>> parts;
		for (size_t i = 0; i < (size_t)vps.num_partitions; i++)
		{
			parts.push_back(partitions[i]);
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

	// A picture has been decoded and is now ready
	picture_ready = true;

	// Now we're finished with the planes, we can reset them ready for whatever new NALs come in
	planes.clear();
	planes[0] = std::map<size_t, matrix<double>>();
	planes[1] = std::map<size_t, matrix<double>>();
	planes[2] = std::map<size_t, matrix<double>>();

	return OVC_DEC_OK;
}

ovc_dec_result ovc_decoder::handle_vps(uint8_t* in_bytes, size_t in_size)
{
	/* VPS FORMAT (30 BYTES) (0x0) */
	/*
	 +---------------+---------------+---------------+---------------+
	 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
	 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 |      0x0      |      0x0      |      0x0      |      0x1      | // Start code
	 +---------------+---------------+-------------------------------+
	 | W F |   W C   | P | S | E |FMT|              N L              |
	 +---------------+---------------+---------------+---------------+
	 |              N P              |              BPP              |
	 +---------------+---------------+---------------+---------------+
	 |           BPP (CONT)          |              L W              |
	 +---------------+---------------+---------------+---------------+
	 |            L W (CONT)         |              L H              |
	 +---------------+---------------+---------------+---------------+
	 |            L H (CONT)         |              C W              |
	 +---------------+---------------+---------------+---------------+
	 |            C W (CONT)         |              C H              |
	 +---------------+---------------+---------------+---------------+
	 |            C H (CONT)         |
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
	 L W = Luma Width     (32)
	 L H = Luma Height    (32)
	 C W = Chroma Width   (32)
	 C H = Chroma Height  (32)
	*/
	if (in_size < 30)
	{
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode VPS (size < 10 (bytes))");
		return OVC_DEC_MALFORMED_NAL_BODY;
	}

	// Parse values for vps
	size_t	 byte_idx = 0;
	uint32_t start_code = 0;
	start_code |= in_bytes[byte_idx++] << 24;
	start_code |= in_bytes[byte_idx++] << 16;
	start_code |= in_bytes[byte_idx++] << 8;
	start_code |= in_bytes[byte_idx++] << 0;

	if (start_code != 0x1)
	{
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode VPS (incorrect start sequence)");
		return OVC_DEC_MALFORMED_NAL_BODY;
	}

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

	vps.luma_width = 0;
	vps.luma_width |= in_bytes[byte_idx++] << 24;
	vps.luma_width |= in_bytes[byte_idx++] << 16;
	vps.luma_width |= in_bytes[byte_idx++] << 8;
	vps.luma_width |= in_bytes[byte_idx++] << 0;

	vps.luma_height = 0;
	vps.luma_height |= in_bytes[byte_idx++] << 24;
	vps.luma_height |= in_bytes[byte_idx++] << 16;
	vps.luma_height |= in_bytes[byte_idx++] << 8;
	vps.luma_height |= in_bytes[byte_idx++] << 0;

	vps.chroma_width = 0;
	vps.chroma_width |= in_bytes[byte_idx++] << 24;
	vps.chroma_width |= in_bytes[byte_idx++] << 16;
	vps.chroma_width |= in_bytes[byte_idx++] << 8;
	vps.chroma_width |= in_bytes[byte_idx++] << 0;

	vps.chroma_height = 0;
	vps.chroma_height |= in_bytes[byte_idx++] << 24;
	vps.chroma_height |= in_bytes[byte_idx++] << 16;
	vps.chroma_height |= in_bytes[byte_idx++] << 8;
	vps.chroma_height |= in_bytes[byte_idx++] << 0;

	vps.is_set = true;

	// Intialise components based on specified vps
	wavelet_recomposer = ovc_wavelet_recomposer_factory::create_wavelet_recomposer(vps.wavelet_family, vps.wavelet_config);
	departitioner = ovc_departitioner_factory::create_departitioner(vps.partition_type);

	return OVC_DEC_OK;
}

ovc_dec_result ovc_decoder::handle_partition(uint8_t* in_bytes, size_t in_size)
{
	/* PARTITION FORMAT (VARIABLE BYTES) (0x1) */
	/*
	+---------------+---------------+---------------+---------------+
	|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|      0x0      |      0x0      |      0x0      |      0x1      | // Start code
	+---------------+---------------+-------------------------------+
	| C |      PARTITION_ID         |              STEP             |
	+---------------+---------------+-------------------------------+
	|         PARTITION DATA ...                                    |
	|                               +---------------+---------------+
	|                               :
	+---------------+---------------|
	*/
	if (in_size < 8)
	{
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode partition (size < 2 (bytes))");
		return OVC_DEC_MALFORMED_NAL_BODY;
	}

	if (!vps.is_set)
	{
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode partition (No VPS received)");
		return OVC_DEC_MISSING_VPS;
	}

	size_t	 byte_idx = 0;
	uint32_t start_code = 0;
	start_code |= in_bytes[byte_idx++] << 24;
	start_code |= in_bytes[byte_idx++] << 16;
	start_code |= in_bytes[byte_idx++] << 8;
	start_code |= in_bytes[byte_idx++] << 0;

	if (start_code != 0x1)
	{
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Unable to decode partition (incorrect start sequence)");
		return OVC_DEC_MALFORMED_NAL_BODY;
	}

	uint8_t partition_byte = 0;
	// clang-format off
	partition_byte = in_bytes[byte_idx++];
	uint8_t component = 0;
    component |= (partition_byte & 0b11000000) >> 6;

    uint16_t partition_id = 0;
    partition_id |= (partition_byte & 0b00111111) << 8;
    partition_id |=        (in_bytes[byte_idx++]) << 0;
	// clang-format on

	if (planes[component].contains(partition_id))
	{
		// This is the second time we're receiving this partition, indicating a new frame being received
		OVC_LOG(LogDecode, OVC_VERBOSITY_WARNING, "Duplicate partition (c: %d, p: %d) received", component, partition_id);
		flush();
	}

	uint16_t step = 0;
	step |= in_bytes[byte_idx++] << 8;
	step |= in_bytes[byte_idx++] << 0;

	// Update data to account for the two start bytes read
	in_bytes += 8;
	in_size -= 8;

	size_t width = size_t(sqrt(vps.num_partitions) * (component == 0 ? vps.luma_width : vps.chroma_width) / vps.num_partitions);
	size_t height = size_t(sqrt(vps.num_partitions) * (component == 0 ? vps.luma_height : vps.chroma_height) / vps.num_partitions);

	// Decode
	std::shared_ptr<ovc_entropy_decoder> entropy_decoder = ovc_entropy_decoder_factory::create_entropy_decoder(vps.entropy_coder);
	//                               num_bytes            num_symbols (bits)
	entropy_decoder->decode(in_bytes, in_size, (size_t)(ceil(width * height * vps.bits_per_pixel)));
	uint8_t* entropy_decoded_bytes = new uint8_t();
	size_t	 entropy_decoded_size = 0;
	entropy_decoder->flush(&entropy_decoded_bytes, &entropy_decoded_size);

	std::shared_ptr<ovc_spiht_decoder> spiht_decoder = ovc_spiht_decoder_factory::create_spiht_decoder(vps.spiht);
	spiht_decoder->decode(entropy_decoded_bytes, entropy_decoded_size, width, height, { .bpp = vps.bits_per_pixel, .num_levels = (size_t)vps.num_levels, .step = step });
	matrix<double> partition = matrix<double>(height, width);
	spiht_decoder->flush(partition);

	// Get the partitions for this plane
	std::map<size_t, matrix<double>>& partitions = planes[component];
	// Assign the partition data to partition_id
	partitions[partition_id] = partition;

	return OVC_DEC_OK;
}

#undef LogDecode