#pragma once

#include <format>
#include <iostream>

#include "verbosity.h"

namespace bvc_logging
{
	static bvc_verbosity verbosity;
}

// TODO (belchy06): Fix this. Macro doesn't seem to respect verbosity
#define LOG(in_category, in_verbosity, in_format, ...)                                            \
	{                                                                                             \
		if (in_verbosity <= bvc_logging::verbosity)                                               \
		{                                                                                         \
			std::cout << in_category << ": " << std::format(in_format, __VA_ARGS__) << std::endl; \
		}                                                                                         \
	}