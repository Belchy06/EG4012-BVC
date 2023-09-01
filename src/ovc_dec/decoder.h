#pragma once

#include <map>

#include "ovc_common/log/log.h"
#include "ovc_common/nal.h"
#include "ovc_common/picture.h"
#include "ovc_common/plane.h"
#include "ovc_common/format.h"
#include "ovc_common/wavelet/wavelet.h"
#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_common/partition/partition.h"
#include "ovc_common/compress/compress.h"
#include "ovc_common/entropy/entropy.h"

#include "ovc_dec/compress/decompressor.h"
#include "ovc_dec/entropy/decoder.h"
#include "ovc_dec/interleave/deinterleaver.h"
#include "ovc_dec/partition/departitioner.h"
#include "ovc_dec/wavelet/recomposer.h"

struct ovc_decoder
{
};

namespace ovc
{
	typedef struct vps
	{
		size_t		  luma_width;
		size_t		  luma_height;
		size_t		  chroma_width;
		size_t		  chroma_height;
		chroma_format format;
		float		  framerate;
		float		  bits_per_pixel;

		uint16_t num_partitions;
		uint16_t num_levels;

		wavelet_family wavelet_family;
		wavelet_config wavelet_config;
		partition	   departitioner_type;
		compression	   decompressor_type;
		entropy_coder  decoder_type;

		bool is_set = false;
	} vps;

	typedef struct pps
	{
		uint8_t	 component;
		uint16_t partition;
		int		 step;

		bool is_set = false;
	} pps;

	typedef enum
	{
		ERROR_CONCEALMENT_SKIP,
		ERROR_CONCEALMENT_AVERAGE_RECEIVED,
		ERROR_CONCEALMENT_AVERAGE_SURROUNDING
	} error_concealment;

	class decoder : public ovc_decoder
	{
	public:
		decoder();

		void set_error_concealment(error_concealment in_error_concealment) { error_concealment_type = in_error_concealment; }

		bool decode_nal(const nal* in_nal_unit);
		bool flush();
		bool get_picture(picture* out_pic);

	private:
		bool handle_vps(uint8_t* in_bytes, size_t in_size);
		bool handle_partition(uint8_t* in_bytes, size_t in_size);

	private:
		error_concealment error_concealment_type;

	private:
		vps		vps;
		bool	initialised;
		bool	picture_ready;
		picture out_picture;

		//     component       partition_id     data
		std::map<size_t, std::map<size_t, matrix<double>>> planes;

		std::shared_ptr<wavelet_recomposer> shared_wavelet_recomposer;
		std::shared_ptr<departitioner>		shared_departitioner;
		std::shared_ptr<decompressor>		shared_decompressor;
		std::shared_ptr<entropy_decoder>	shared_entropy_decoder;
	};
} // namespace ovc