#pragma once

#include <stdint.h>

#include "ovc_common/format.h"

class ovc_dec_config
{
public:
	size_t			  width;
	size_t			  height;
	ovc_chroma_format format;
	double			  framerate;
};