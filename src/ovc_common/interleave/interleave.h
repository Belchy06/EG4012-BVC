#pragma once

#include <string>

typedef enum
{
	OVC_INTERLEAVE_SKIP,
	OVC_INTERLEAVE_RANDOM
} ovc_interleave;

inline std::string interleave_to_string(ovc_interleave in_interleave)
{
	switch (in_interleave)
	{
		case OVC_INTERLEAVE_SKIP:
			return "OVC_INTERLEAVE_SKIP";
		case OVC_INTERLEAVE_RANDOM:
			return "OVC_INTERLEAVE_RANDOM";
		default:
			return "";
	}
}