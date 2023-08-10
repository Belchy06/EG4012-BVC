#pragma once

#include <stdint.h>

#include "bvc_common/format.h"

class bvc_enc_config
{
public:
	size_t			  width;
	size_t			  height;
	bvc_chroma_format format;
	double			  framerate;
};