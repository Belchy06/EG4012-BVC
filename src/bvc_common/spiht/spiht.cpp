#include "bvc_common/spiht/spiht.h"

bvc_spiht_set::bvc_spiht_set(size_t in_x, size_t in_y, bvc_spiht_type in_type)
	: x(in_x)
	, y(in_y)
	, type(in_type)
{
}

bvc_spiht_pixel::bvc_spiht_pixel(size_t in_x, size_t in_y)
	: x(in_x)
	, y(in_y)
{
}