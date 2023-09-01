#pragma once

#include <string>

namespace ovc
{
	typedef enum
	{
		FEC_SKIP,
		FEC_REED_SOLOMON,
	} fec;

	inline std::string fec_to_string(fec in_fec)
	{
		switch (in_fec)
		{
			case FEC_SKIP:
				return "FEC_SKIP";
			case FEC_REED_SOLOMON:
				return "FEC_REED_SOLOMON";
			default:
				return "";
		}
	}
} // namespace ovc
