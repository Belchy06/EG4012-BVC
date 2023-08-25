#pragma once

#include <string>

#include "ovc_common/spiht/spiht_config.h"

typedef enum
{
	OVC_SPIHT_SKIP,
	OVC_SPIHT_ENABLE,
} ovc_spiht;

inline std::string spiht_to_string(ovc_spiht in_spiht)
{
	switch (in_spiht)
	{
		case OVC_SPIHT_SKIP:
			return "OVC_SPIHT_SKIP";
		case OVC_SPIHT_ENABLE:
			return "OVC_SPIHT_ENABLE";
		default:
			return "";
	}
}

typedef enum
{
	OVC_SPIHT_TYPE_A,
	OVC_SPIHT_TYPE_B
} ovc_spiht_type;

class ovc_spiht_set
{
public:
	ovc_spiht_set(size_t in_x, size_t in_y, ovc_spiht_type in_type);

public:
	ovc_spiht_type type;
	size_t		   x;
	size_t		   y;
};

class ovc_spiht_pixel
{
public:
	ovc_spiht_pixel(size_t in_x, size_t in_y);

public:
	size_t x;
	size_t y;
};