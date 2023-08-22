#include "ovc_common/util/util.h"

size_t util::scale_x(size_t in_x, ovc_chroma_format in_format)
{
	switch (in_format)
	{
		case OVC_CHROMA_FORMAT_MONOCHROME:
			return 0;
		case OVC_CHROMA_FORMAT_444:
			return in_x;
		case OVC_CHROMA_FORMAT_420:
		case OVC_CHROMA_FORMAT_422:
			return in_x >> 1;
		default:
			return 0;
	}
}

size_t util::scale_y(size_t in_y, ovc_chroma_format in_format)
{
	switch (in_format)
	{
		case OVC_CHROMA_FORMAT_MONOCHROME:
			return 0;
		case OVC_CHROMA_FORMAT_444:
		case OVC_CHROMA_FORMAT_422:
			return in_y;
		case OVC_CHROMA_FORMAT_420:
			return in_y >> 1;
		default:
			return 0;
	}
}