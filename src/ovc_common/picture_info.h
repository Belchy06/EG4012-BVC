#pragma once

#include <stdint.h>

#include "ovc_common/format.h"

class ovc_picture_info
{
public:
	size_t			  width;
	size_t			  height;
	ovc_chroma_format format;
	double			  framerate;
	size_t			  bit_depth;
};