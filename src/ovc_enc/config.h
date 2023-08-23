#pragma once

#include <stdint.h>

#include "ovc_common/format.h"
#include "ovc_common/entropy/entropy.h"
#include "ovc_common/partition/partition.h"
#include "ovc_common/spiht/spiht.h"
#include "ovc_common/wavelet/wavelet.h"

class ovc_enc_config
{
public:
	size_t			  width;
	size_t			  height;
	ovc_chroma_format format;
	float			  framerate;
	float			  bits_per_pixel;

	int num_streams_exp = -1;
	int num_levels = -1;

	ovc_wavelet_family wavelet_family;
	ovc_wavelet_config wavelet_config;
	ovc_partition	   partition_type;
	ovc_spiht		   spiht;
	ovc_entropy_coder  entropy_coder;

	bool repeat_vps;
};