#pragma once

#include <string>

typedef enum
{
	OVC_FEC_SKIP,
	OVC_FEC_REED_SOLOMON,
} ovc_fec;

inline std::string fec_to_string(ovc_fec in_fec)
{
	switch (in_fec)
	{
		case OVC_FEC_SKIP:
			return "OVC_FEC_SKIP";
		case OVC_FEC_REED_SOLOMON:
			return "OVC_FEC_REED_SOLOMON";
		default:
			return "";
	}
}