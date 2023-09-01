#pragma once

#include <stdint.h>

namespace ovc
{
	typedef enum : uint8_t
	{
		NAL_TYPE_VPS,
		NAL_TYPE_PARTITION,
	} nal_type;

	typedef struct nal
	{
		uint8_t* bytes;
		size_t	 size;
	} nal;
} // namespace ovc
