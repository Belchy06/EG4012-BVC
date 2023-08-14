#pragma once

#include <stdint.h>

#include "bvc_common/format.h"
#include "bvc_common/entropy.h"

class bvc_enc_config
{
public:
	size_t			  width;
	size_t			  height;
	bvc_chroma_format format;
	bvc_entropy_coder entropy_coder;
	double			  framerate;
};