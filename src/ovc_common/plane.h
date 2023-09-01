#pragma once

namespace ovc
{
	typedef struct plane
	{
		uint8_t* data;
		size_t	 width;
		size_t	 height;
		size_t	 bit_depth;
	} plane;
} // namespace ovc
