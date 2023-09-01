#pragma once

#include <string>

namespace ovc
{
	typedef enum
	{
		VERBOSITY_SILENT,
		VERBOSITY_ERROR,
		VERBOSITY_WARNING,
		VERBOSITY_INFO,
		VERBOSITY_NOTICE,
		VERBOSITY_VERBOSE,
		VERBOSITY_DETAILS
	} verbosity;

	inline std::string verbosity_to_string(verbosity in_verbosity)
	{
		switch (in_verbosity)
		{
			case VERBOSITY_SILENT:
				return "silent";
			case VERBOSITY_ERROR:
				return "error";
			case VERBOSITY_WARNING:
				return "warning";
			case VERBOSITY_INFO:
				return "info";
			case VERBOSITY_NOTICE:
				return "notice";
			case VERBOSITY_VERBOSE:
				return "verbose";
			case VERBOSITY_DETAILS:
				return "details";
			default:
				return "unknown";
		}
	}
} // namespace ovc