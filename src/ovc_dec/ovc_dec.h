#pragma once

#include <map>

#include "ovc_common/log/log.h"
#include "ovc_common/nal.h"
#include "ovc_common/picture.h"
#include "ovc_common/plane.h"
#include "ovc_dec/entropy/decoder_factory.h"
#include "ovc_dec/partition/departitioner_factory.h"
#include "ovc_dec/spiht/decoder_factory.h"
#include "ovc_dec/wavelet/recomposer_factory.h"
#include "ovc_dec/result.h"
#include "ovc_dec/config.h"

class ovc_vps
{
public:
	size_t			  luma_width;
	size_t			  luma_height;
	size_t			  chroma_width;
	size_t			  chroma_height;
	ovc_chroma_format format;
	float			  framerate;
	float			  bits_per_pixel;

	uint16_t num_partitions;
	uint16_t num_levels;

	ovc_wavelet_family wavelet_family;
	ovc_wavelet_config wavelet_config;
	ovc_partition	   partition_type;
	ovc_spiht		   spiht;
	ovc_entropy_coder  entropy_coder;

	bool is_set = false;
};

class ovc_pps
{
public:
	uint8_t	 component;
	uint16_t partition;
	int		 step;

	bool is_set = false;
};

class ovc_decoder
{
public:
	ovc_decoder();

	ovc_dec_result init(ovc_dec_config* in_config);
	void		   set_logging_callback(ovc_logging_callback in_callback);
	ovc_dec_result decode_nal(const ovc_nal* in_nal_unit);
	ovc_dec_result flush();
	ovc_dec_result get_picture(ovc_picture* out_picture);

private:
	ovc_dec_result handle_vps(uint8_t* in_bytes, size_t in_size);
	ovc_dec_result handle_partition(uint8_t* in_bytes, size_t in_size);

private:
	ovc_dec_config config;
	ovc_vps		   vps;
	bool		   initialised;
	bool		   picture_ready;
	ovc_picture	   picture;

	//     component       partition_id     data
	std::map<size_t, std::map<size_t, matrix<double>>> planes;

	std::shared_ptr<ovc_wavelet_recomposer> wavelet_recomposer;
	std::shared_ptr<ovc_departitioner>		departitioner;
	std::shared_ptr<ovc_spiht_decoder>		spiht_decoder;
	std::shared_ptr<ovc_entropy_decoder>	entropy_decoder;
};