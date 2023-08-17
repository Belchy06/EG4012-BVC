#pragma once

#include <stdint.h>

#include "bvc_common/format.h"
#include "bvc_common/entropy/entropy.h"
#include "bvc_common/partition/partition.h"
#include "bvc_common/wavelet/wavelet.h"

class bvc_enc_config
{
public:
	size_t			  width;
	size_t			  height;
	bvc_chroma_format format;
	double			  framerate;
	double			  bits_per_pixel;

	size_t num_streams;
	size_t num_levels;

	bvc_entropy_coder entropy_coder;

	bvc_wavelet		   wavelet_family;
	bvc_wavelet_config wavelet_config;

	bvc_partition partition_type;
};