#pragma once

namespace ovc
{
	typedef enum
	{
		SPIHT_TYPE_A,
		SPIHT_TYPE_B
	} spiht_type;

	class spiht_set
	{
	public:
		spiht_set(size_t in_x, size_t in_y, spiht_type in_type)
			: x(in_x), y(in_y), type(in_type){};

	public:
		spiht_type type;
		size_t	   x;
		size_t	   y;
	};

	class spiht_pixel
	{
	public:
		spiht_pixel(size_t in_x, size_t in_y)
			: x(in_x), y(in_y){};

	public:
		size_t x;
		size_t y;
	};
} // namespace ovc