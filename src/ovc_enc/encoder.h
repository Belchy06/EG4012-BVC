#pragma once

#include <memory>
#include <mutex>

#include "ovc_common/compress/compress.h"
#include "ovc_common/entropy/entropy.h"
#include "ovc_common/interleave/interleave.h"
#include "ovc_common/log/log.h"
#include "ovc_common/nal.h"
#include "ovc_common/partition/partition.h"
#include "ovc_common/picture.h"
#include "ovc_common/wavelet/wavelet.h"
#include "ovc_common/wavelet/wavelet_config.h"

#include "ovc_enc/interleave/interleaver.h"

struct ovc_encoder
{
};

namespace ovc
{
	class encoder : public ovc_encoder
	{
	public:
		encoder();

		// config
		void set_width(size_t in_width) { width = in_width; }
		void set_height(size_t in_height) { height = in_height; }
		void set_format(chroma_format in_format) { format = in_format; }
		void set_framerate(float in_framerate) { framerate = in_framerate; }
		void set_bits_per_pixel(float in_bits_per_pixel) { bits_per_pixel = in_bits_per_pixel; }
		void set_seed(uint16_t in_seed) { seed = in_seed; }
		void set_num_parts_exp(int in_num_parts_exp) { num_parts_exp = in_num_parts_exp; }
		void set_num_levels(int in_num_levels) { num_levels = in_num_levels; }
		void set_wavelet_family(wavelet_family in_wavelet_family) { wavelet_decomposer_family = in_wavelet_family; }
		void set_wavelet_config(wavelet_config in_wavelet_config) { wavelet_decomposer_config = in_wavelet_config; }
		void set_partitioner(partition in_partition) { partitioner_type = in_partition; }
		void set_compressor(compression in_compression) { compressor_type = in_compression; }
		void set_entropy_coder(entropy_coder in_entropy_coder) { coder_type = in_entropy_coder; }
		void set_interleaver(interleave in_interleave) { interleaver_type = in_interleave; }
		void set_repeat_vps(bool in_repeat_vps) { repeat_vps = in_repeat_vps; }
		bool set_multithreading(bool in_multithreading) { multithreading = in_multithreading; }

		//
		bool				   encode(picture* in_picture);
		std::vector<ovc::nal>& get_output_nals();

	private:
		void encode_component(picture* in_picture, uint8_t in_component);
		void encode_partition(matrix<double> in_partition, uint16_t in_partition_id, uint8_t in_component);
		void construct_and_output_vps();

	private:
		// config
		size_t		  width;
		size_t		  height;
		chroma_format format;
		float		  framerate;
		float		  bits_per_pixel;
		uint16_t	  seed;

		int num_parts_exp = -1;
		int num_levels = -1;

		wavelet_family wavelet_decomposer_family;
		wavelet_config wavelet_decomposer_config;
		partition	   partitioner_type;
		compression	   compressor_type;
		entropy_coder  coder_type;
		interleave	   interleaver_type;

		bool repeat_vps;
		bool multithreading;

	private:
		bool initialised;
		bool send_vps;

		std::mutex			  output_nals_mutex;
		std::vector<ovc::nal> output_nals;

		std::shared_ptr<interleaver> nal_interleaver;
	};
} // namespace ovc