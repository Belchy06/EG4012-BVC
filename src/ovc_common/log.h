#pragma once

#include <format>
#include <iostream>

#include "verbosity.h"

namespace ovc_logging
{
	inline ovc_verbosity verbosity;
}

// TODO (belchy06): Fix this. Macro doesn't seem to respect verbosity
#define LOG(in_category, in_verbosity, in_format, ...)                                                                                                  \
	{                                                                                                                                                   \
		if (in_verbosity <= ovc_logging::verbosity)                                                                                                     \
		{                                                                                                                                               \
			std::cout << "[ " << verbosity_to_string(in_verbosity) << " ] " << in_category << ": " << std::format(in_format, __VA_ARGS__) << std::endl; \
		}                                                                                                                                               \
	}