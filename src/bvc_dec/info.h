#pragma once

#include <stdint.h>

#include "bvc_common/format.h"

class bvc_image_info
{
public:
	size_t			  width;
	size_t			  height;
	bvc_chroma_format format;
	double			  framerate;
	size_t			  bit_depth;
};