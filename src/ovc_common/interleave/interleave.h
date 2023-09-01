#pragma once

#include <string>

namespace ovc
{
	typedef enum
	{
		INTERLEAVE_SKIP,
		INTERLEAVE_RANDOM
	} interleave;

	inline std::string interleave_to_string(interleave in_interleave)
	{
		switch (in_interleave)
		{
			case INTERLEAVE_SKIP:
				return "INTERLEAVE_SKIP";
			case INTERLEAVE_RANDOM:
				return "INTERLEAVE_RANDOM";
			default:
				return "";
		}
	}
} // namespace ovc
