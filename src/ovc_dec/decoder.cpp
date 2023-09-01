#include <cmath>
#include <cstring>

#include "ovc_dec/decoder.h"

#include "ovc_dec/compress/decompressor_factory.h"
#include "ovc_dec/entropy/decoder_factory.h"
#include "ovc_dec/interleave/deinterleaver_factory.h"

#include "ovc_dec/partition/departitioner_factory.h"
#include "ovc_dec/wavelet/recomposer_factory.h"

#define LogDecode "LogDecode"

namespace ovc
{
	decoder::decoder()
		: picture_ready(false)
		, initialised(false)
	{
	}

	bool decoder::decode_nal(const nal* in_nal_unit)
	{
		if (!initialised)
		{
			picture_ready = false;
			out_picture = picture();

			planes.clear();
			planes[0] = std::map<size_t, matrix<double>>();
			planes[1] = std::map<size_t, matrix<double>>();
			planes[2] = std::map<size_t, matrix<double>>();

			initialised = true;
		}

		uint8_t* nal_bytes = new uint8_t[in_nal_unit->size]{ 0 };
		size_t	 nal_size = in_nal_unit->size;
		memcpy(nal_bytes, in_nal_unit->bytes, in_nal_unit->size);

		if (nal_size < 2)
		{
			LOG(LogDecode, VERBOSITY_WARNING, "Dropping NAL due to malformed header (size < 2 (bytes))");
			return false;
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
	nal_type type = (nal_type)((type_byte & 0b00111111) >> 0);
    nal_bytes += 2;
    nal_size -= 2;
		// clang-format on
		if ((start_byte & zero_bits) != 0)
		{
			LOG(LogDecode, VERBOSITY_WARNING, "Dropping NAL due to malformed header (start & res) != 0");
			return false;
		}

		switch (type)
		{
			case NAL_TYPE_VPS:
				return handle_vps(nal_bytes, nal_size);
			case NAL_TYPE_PARTITION:
				return handle_partition(nal_bytes, nal_size);
			default:
				LOG(LogDecode, VERBOSITY_WARNING, "Unknown NAL type");
				return false;
		}
	}

	bool decoder::get_picture(picture* out_pic)
	{
		if (!picture_ready)
		{
			// We haven't received enough NALs to produce a full picture
			return false;
		}

		// TODO (belchy06): Parse headers
		*out_pic = std::move(out_picture);
		out_pic->framerate = 29.97f;

		picture_ready = false;

		return true;
	}

	bool decoder::flush()
	{
		// Reset our picture memory
		out_picture = picture();

		for (uint8_t component = 0; component < (uint8_t)(vps.format == CHROMA_FORMAT_MONOCHROME ? 1 : 3); component++)
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
					if (error_concealment_type == ERROR_CONCEALMENT_AVERAGE_RECEIVED)
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
						LOG(LogDecode, VERBOSITY_WARNING, "Supplementing c: %d, p: %d with the average of surrounding values", component, missing_key);
					}
					else if (error_concealment_type == ERROR_CONCEALMENT_AVERAGE_SURROUNDING)
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
						LOG(LogDecode, VERBOSITY_WARNING, "Supplementing c: %d, p: %d with the average of surrounding values in a + shape", component, missing_key);
					}
					else if (error_concealment_type == ERROR_CONCEALMENT_SKIP)
					{
						LOG(LogDecode, VERBOSITY_WARNING, "Supplementing c: %d, p: %d with a zero matrix", component, missing_key);
					}

					partitions[missing_key] = supplement_partition;
				}
			}

			std::vector<matrix<double>> parts;
			for (size_t i = 0; i < (size_t)vps.num_partitions; i++)
			{
				parts.push_back(partitions[i]);
			}

			matrix<double> full = shared_departitioner->departition(parts, vps.num_levels);

			wavelet_decomposition_2d<double> decomposition = wavelet_decomposition_2d<double>::from_matrix(full, vps.num_levels);
			matrix<double>					 image = shared_wavelet_recomposer->recompose(decomposition, full.size());

			std::vector<uint8_t> plane_data;
			for (size_t y = 0; y < image.get_num_rows(); y++)
			{
				for (size_t x = 0; x < image.get_num_columns(); x++)
				{
					double pixel = trunc(image(y, x));
					plane_data.push_back((uint8_t)pixel);
				}
			}

			plane plane;
			plane.data = new uint8_t[image.get_num_rows() * image.get_num_columns()]{ 0 };

			plane.height = image.get_num_rows();
			plane.width = image.get_num_columns();
			plane.bit_depth = 8;
			memcpy(plane.data, plane_data.data(), plane_data.size());

			out_picture.planes[component] = plane;
			out_picture.format = vps.format;
		}

		// A picture has been decoded and is now ready
		picture_ready = true;

		// Now we're finished with the planes, we can reset them ready for whatever new NALs come in
		planes.clear();
		planes[0] = std::map<size_t, matrix<double>>();
		planes[1] = std::map<size_t, matrix<double>>();
		planes[2] = std::map<size_t, matrix<double>>();

		return true;
	}

	bool decoder::handle_vps(uint8_t* in_bytes, size_t in_size)
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
			LOG(LogDecode, VERBOSITY_WARNING, "Unable to decode VPS (size < 10 (bytes))");
			return false;
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
			LOG(LogDecode, VERBOSITY_WARNING, "Unable to decode VPS (incorrect start sequence)");
			return false;
		}

		uint8_t vps_byte = 0;
		// clang-format off
	    vps_byte = in_bytes[byte_idx++];
	    vps.wavelet_family =  (wavelet_family)((vps_byte & 0b11100000) >> 5);
	    vps.wavelet_config =  { .value = (uint8_t)((vps_byte & 0b00011111) >> 0) };
		// clang-format on

		// clang-format off
	    vps_byte = in_bytes[byte_idx++];
	    vps.departitioner_type = (partition)    ((vps_byte & 0b11000000) >> 6);
	    vps.decompressor_type  = (compression)  ((vps_byte & 0b00110000) >> 4);
	    vps.decoder_type       = (entropy_coder)((vps_byte & 0b00001100) >> 2);
        vps.format             = (chroma_format)((vps_byte & 0b00000011) >> 0);
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
		shared_wavelet_recomposer = wavelet_recomposer_factory::create_wavelet_recomposer(vps.wavelet_family, vps.wavelet_config);
		shared_departitioner = departitioner_factory::create_departitioner(vps.departitioner_type);
		shared_decompressor = decompressor_factory::create_decompressor(vps.decompressor_type);
		shared_entropy_decoder = entropy_decoder_factory::create_entropy_decoder(vps.decoder_type);

		return true;
	}

	bool decoder::handle_partition(uint8_t* in_bytes, size_t in_size)
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
			LOG(LogDecode, VERBOSITY_WARNING, "Unable to decode partition (size < 2 (bytes))");
			return false;
		}

		if (!vps.is_set)
		{
			LOG(LogDecode, VERBOSITY_WARNING, "Unable to decode partition (No VPS received)");
			return false;
		}

		size_t	 byte_idx = 0;
		uint32_t start_code = 0;
		start_code |= in_bytes[byte_idx++] << 24;
		start_code |= in_bytes[byte_idx++] << 16;
		start_code |= in_bytes[byte_idx++] << 8;
		start_code |= in_bytes[byte_idx++] << 0;

		if (start_code != 0x1)
		{
			LOG(LogDecode, VERBOSITY_WARNING, "Unable to decode partition (incorrect start sequence)");
			return false;
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
			LOG(LogDecode, VERBOSITY_WARNING, "Duplicate partition (c: %d, p: %d) received", component, partition_id);
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
		//                               num_bytes            num_symbols (bits)
		shared_entropy_decoder->decode(in_bytes, in_size, (size_t)(ceil(width * height * vps.bits_per_pixel)));
		uint8_t* entropy_decoded_bytes = new uint8_t();
		size_t	 entropy_decoded_size = 0;
		shared_entropy_decoder->flush(&entropy_decoded_bytes, &entropy_decoded_size);

		shared_decompressor->decompress(entropy_decoded_bytes, entropy_decoded_size, width, height, { .bpp = vps.bits_per_pixel, .num_levels = (size_t)vps.num_levels, .step = step });
		matrix<double> partition = matrix<double>(height, width);
		shared_decompressor->flush(partition);

		// Get the partitions for this plane
		std::map<size_t, matrix<double>>& partitions = planes[component];
		// Assign the partition data to partition_id
		partitions[partition_id] = partition;

		return true;
	}
} // namespace ovc
#undef LogDecode