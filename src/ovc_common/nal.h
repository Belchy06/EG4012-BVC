#pragma once

#include <stdint.h>

typedef enum : uint8_t
{
	OVC_NAL_TYPE_VPS,
	OVC_NAL_TYPE_PARTITION,
} ovc_nal_type;

class ovc_nal
{
public:
	uint8_t* bytes;
	size_t	 size;
};