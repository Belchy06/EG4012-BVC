#pragma once

#include "ovc_common/format.h"

class util
{
public:
	static size_t scale_x(size_t in_x, ovc_chroma_format in_format);
	static size_t scale_y(size_t in_y, ovc_chroma_format in_format);
};