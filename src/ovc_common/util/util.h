#pragma once

#include "ovc_common/format.h"

namespace ovc
{
	static inline size_t scale_x(size_t in_x, chroma_format in_format)
	{
		switch (in_format)
		{
			case CHROMA_FORMAT_MONOCHROME:
				return 0;
			case CHROMA_FORMAT_444:
				return in_x;
			case CHROMA_FORMAT_420:
			case CHROMA_FORMAT_422:
				return in_x >> 1;
			default:
				return 0;
		}
	}
	static inline size_t scale_y(size_t in_y, chroma_format in_format)
	{
		switch (in_format)
		{
			case CHROMA_FORMAT_MONOCHROME:
				return 0;
			case CHROMA_FORMAT_444:
			case CHROMA_FORMAT_422:
				return in_y;
			case CHROMA_FORMAT_420:
				return in_y >> 1;
			default:
				return 0;
		}
	}
} // namespace ovc