#include <bitset>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

#include "ovc_common/util/util.h"
#include "ovc_enc/encoder.h"
#include "ovc_enc/entropy/encoder_factory.h"
#include "ovc_enc/interleave/interleaver_factory.h"
#include "ovc_enc/partition/partitioner_factory.h"
#include "ovc_enc/compress/compressor_factory.h"
#include "ovc_enc/wavelet/decomposer_factory.h"

namespace ovc
{
	encoder::encoder()
		: send_vps(true)
		, initialised(false)
	{
	}

	bool encoder::encode(picture* in_picture)
	{
		if (!initialised)
		{
			interleave_config interleaver_config;
			interleaver_config.seed = seed;
			nal_interleaver = interleaver_factory::create_interleaver(interleaver_type, interleaver_config);
			initialised = true;
		}

		output_nals.clear();

		std::vector<std::thread> threads;
		for (uint8_t component = 0; component < (uint8_t)(format == CHROMA_FORMAT_MONOCHROME ? 1 : 3); component++)
		{
			if (multithreading)
			{
				std::thread thread = std::thread([in_picture, component, this]() {
					encode_component(in_picture, component);
				});

				threads.push_back(std::move(thread));
			}
			else
			{
				encode_component(in_picture, component);
			}
		}

		for (std::thread& thread : threads)
		{
			thread.join();
		}

		// Interleaver shuffles (or not) the order of the nals when output
		output_nals = nal_interleaver->interleave(output_nals);

		// VPS is 1 per video (or picture if configured for such)
		// VPS NAL
		// This must be the first NAL in an AU
		construct_and_output_vps();

		return true;
	}

	std::vector<ovc::nal>& encoder::get_output_nals()
	{
		return output_nals;
	}

	void encoder::encode_component(ovc::picture* in_picture, uint8_t in_component)
	{
		size_t				plane_width = (in_component == 0 ? width : ovc::scale_x(width, format));
		size_t				plane_height = (in_component == 0 ? height : ovc::scale_y(height, format));
		matrix<double>		plane_matrix(plane_height, plane_width);
		std::vector<double> plane_data;
		for (size_t x = 0; x < plane_width; x++)
		{
			for (size_t y = 0; y < plane_height; y++)
			{
				uint8_t* plane = in_picture->planes[in_component].data;
				plane_data.push_back((double)plane[x + y * plane_width]);
			}
		}

		plane_matrix.set_data(plane_data);

		std::shared_ptr<wavelet_decomposer> shared_decomposer = wavelet_decomposer_factory::create_wavelet_decomposer(wavelet_decomposer_family, wavelet_decomposer_config);
		std::shared_ptr<partitioner>		shared_partitioner = partitioner_factory::create_partitioner(partitioner_type);

		ovc::wavelet_decomposition_2d<double> decomposition = shared_decomposer->decompose(plane_matrix, num_levels);
		matrix<double>						  decomp_matrix = decomposition.to_matrix();

		std::vector<matrix<double>> partitions = shared_partitioner->partition(decomp_matrix, num_levels, (size_t)pow(4, num_parts_exp));

		std::vector<std::thread> threads;
		for (uint16_t i = 0; i < partitions.size(); i++)
		{
			matrix<double> partition = partitions[i];
			if (multithreading)
			{
				std::thread thread = std::thread([partition, i, in_component, this]() {
					encode_partition(partition, i, in_component);
				});

				threads.push_back(std::move(thread));
			}
			else
			{
				encode_partition(partition, i, in_component);
			}
		}

		for (std::thread& thread : threads)
		{
			thread.join();
		}
	}

	void encoder::encode_partition(matrix<double> in_partition, uint16_t in_partition_id, uint8_t in_component)
	{
		size_t part_width = in_partition.get_num_columns();
		size_t part_height = in_partition.get_num_rows();

		std::shared_ptr<compressor>		 compressor = compressor_factory::create_compressor(compressor_type);
		std::shared_ptr<entropy_encoder> entropy_encoder = entropy_encoder_factory::create_entropy_encoder(coder_type);

		compressor->compress(in_partition, { .bpp = bits_per_pixel, .num_levels = (size_t)num_levels });
		size_t	 compressor_byte_length = ((size_t)ceil(part_width * part_height * bits_per_pixel) >> 3);
		uint8_t* compressor_bitstream = new uint8_t();
		uint16_t compressor_step_size = 0;
		compressor->flush(&compressor_bitstream, &compressor_step_size);

		entropy_encoder->encode(compressor_bitstream, compressor_byte_length);
		uint8_t* entropy_bitstream = new uint8_t();
		size_t	 entropy_byte_length = 0;
		entropy_encoder->flush(&entropy_bitstream, &entropy_byte_length);

		std::vector<uint8_t> bytes;
		for (size_t j = 0; j < entropy_byte_length; j++)
		{
			bytes.push_back(entropy_bitstream[j]);
		}

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
		nal_header_byte |= (NAL_TYPE_PARTITION << 0) & 0b00111111; // T
		nal_header.push_back(nal_header_byte);
		// clang-format on

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

		std::vector<uint8_t> partition_header;
		partition_header.push_back(0x0);
		partition_header.push_back(0x0);
		partition_header.push_back(0x0);
		partition_header.push_back(0x1);

		uint8_t partition_header_byte;
		partition_header_byte = 0;
		// clang-format off
		partition_header_byte |= (in_component    << 6) & 0b11000000;
		partition_header_byte |= (in_partition_id >> 8) & 0b00111111;
		partition_header.push_back(partition_header_byte);
		// clang-format on

		partition_header_byte = 0;
		// clang-format off
		partition_header_byte |= (in_partition_id >> 0) & 0b11111111;
		partition_header.push_back(partition_header_byte);
		// clang-format on

		partition_header.push_back((compressor_step_size >> 8) & 0b11111111);
		partition_header.push_back((compressor_step_size >> 0) & 0b11111111);

		std::vector<uint8_t> nal_bytes;
		nal_bytes.reserve(nal_header.size() + partition_header.size() + bytes.size());
		nal_bytes.insert(nal_bytes.end(), nal_header.begin(), nal_header.end());
		nal_bytes.insert(nal_bytes.end(), partition_header.begin(), partition_header.end());
		nal_bytes.insert(nal_bytes.end(), bytes.begin(), bytes.end());

		ovc::nal nal;
		nal.bytes = new uint8_t[nal_bytes.size()]{ 0 };
		memcpy(nal.bytes, nal_bytes.data(), nal_bytes.size());
		nal.size = nal_bytes.size();

		output_nals_mutex.lock();
		output_nals.push_back(nal);
		output_nals_mutex.unlock();
	}

	void encoder::construct_and_output_vps()
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
			header_byte |= (NAL_TYPE_VPS << 0) & 0b00111111; // T
			header.push_back(header_byte);
			// clang-format on

			/* VPS FORMAT (26 BYTES) (0x0) */
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
			std::vector<uint8_t> vps;
			vps.push_back(0x0);
			vps.push_back(0x0);
			vps.push_back(0x0);
			vps.push_back(0x1);

			uint8_t vps_byte;
			vps_byte = 0;
			// clang-format off
			vps_byte |= (wavelet_decomposer_family       << 5) & 0b11100000;
			vps_byte |= (wavelet_decomposer_config.value << 0) & 0b00011111;
			vps.push_back(vps_byte);
			// clang-format on

			vps_byte = 0;
			// clang-format off
			vps_byte |= (partitioner_type << 6) & 0b11000000;
			vps_byte |= (compressor_type  << 4) & 0b00110000;
			vps_byte |= (coder_type       << 2) & 0b00001100;
			vps_byte |= (format         << 0) & 0b00000011;
			vps.push_back(vps_byte);
			// clang-format on

			vps.push_back((uint8_t)(num_levels >> 8));
			vps.push_back((uint8_t)(num_levels >> 0));

			vps.push_back((uint8_t)((size_t)pow(4, num_parts_exp) >> 8));
			vps.push_back((uint8_t)((size_t)pow(4, num_parts_exp) >> 0));

			uint8_t* bpp_arr = reinterpret_cast<uint8_t*>(&bits_per_pixel);
			vps.push_back(bpp_arr[3]);
			vps.push_back(bpp_arr[2]);
			vps.push_back(bpp_arr[1]);
			vps.push_back(bpp_arr[0]);

			// luma width
			// clang-format off
			vps.push_back((width >> 24) & 0b11111111);
			vps.push_back((width >> 16) & 0b11111111);
			vps.push_back((width >>  8) & 0b11111111);
			vps.push_back((width >>  0) & 0b11111111);
			// clang-format on

			// luma height
			// clang-format off
			vps.push_back((height >> 24) & 0b11111111);
			vps.push_back((height >> 16) & 0b11111111);
			vps.push_back((height >>  8) & 0b11111111);
			vps.push_back((height >>  0) & 0b11111111);
			// clang-format on

			// chroma width
			// clang-format off
			vps.push_back((ovc::scale_x(width, format) >> 24) & 0b11111111);
			vps.push_back((ovc::scale_x(width, format) >> 16) & 0b11111111);
			vps.push_back((ovc::scale_x(width, format) >>  8) & 0b11111111);
			vps.push_back((ovc::scale_x(width, format) >>  0) & 0b11111111);
			// clang-format on

			// chroma height
			// clang-format off
			vps.push_back((ovc::scale_y(height, format) >> 24) & 0b11111111);
			vps.push_back((ovc::scale_y(height, format) >> 16) & 0b11111111);
			vps.push_back((ovc::scale_y(height, format) >>  8) & 0b11111111);
			vps.push_back((ovc::scale_y(height, format) >>  0) & 0b11111111);
			// clang-format on

			std::vector<uint8_t> nal_bytes;
			nal_bytes.reserve(header.size() + vps.size());
			nal_bytes.insert(nal_bytes.end(), header.begin(), header.end());
			nal_bytes.insert(nal_bytes.end(), vps.begin(), vps.end());

			ovc::nal nal;
			nal.bytes = new uint8_t[nal_bytes.size()]{ 0 };
			memcpy(nal.bytes, nal_bytes.data(), nal_bytes.size());
			nal.size = nal_bytes.size();

			output_nals_mutex.lock();
			output_nals.insert(output_nals.begin(), nal);
			output_nals_mutex.unlock();
		}

		send_vps &= repeat_vps;
	}
} // namespace ovc