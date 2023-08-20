#include "ovc_common/spiht/spiht.h"

ovc_spiht_set::ovc_spiht_set(size_t in_x, size_t in_y, ovc_spiht_type in_type)
	: x(in_x)
	, y(in_y)
	, type(in_type)
{
}

ovc_spiht_pixel::ovc_spiht_pixel(size_t in_x, size_t in_y)
	: x(in_x)
	, y(in_y)
{
}