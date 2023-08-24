#pragma once

#include <string>

typedef enum
{
	OVC_VERBOSITY_SILENT,
	OVC_VERBOSITY_ERROR,
	OVC_VERBOSITY_WARNING,
	OVC_VERBOSITY_INFO,
	OVC_VERBOSITY_NOTICE,
	OVC_VERBOSITY_VERBOSE,
	OVC_VERBOSITY_DETAILS
} ovc_verbosity;

inline std::string verbosity_to_string(ovc_verbosity in_verbosity)
{
	switch (in_verbosity)
	{
		case OVC_VERBOSITY_SILENT:
			return "silent";
		case OVC_VERBOSITY_ERROR:
			return "error";
		case OVC_VERBOSITY_WARNING:
			return "warning";
		case OVC_VERBOSITY_INFO:
			return "info";
		case OVC_VERBOSITY_NOTICE:
			return "notice";
		case OVC_VERBOSITY_VERBOSE:
			return "verbose";
		case OVC_VERBOSITY_DETAILS:
			return "details";
		default:
			return "unknown";
	}
}