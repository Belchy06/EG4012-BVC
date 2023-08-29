#pragma once

#include <cstdarg>
#include <format>
#include <functional>
#include <iostream>
#include <string>

#include "ovc_common/verbosity.h"

typedef void (*ovc_logging_callback)(int, const char*, va_list);

namespace ovc_logging
{
	inline ovc_verbosity								  verbosity;
	extern std::function<void(int, const char*, va_list)> logging_function;
} // namespace ovc_logging

inline void DEFAULT_OVC_LOG(int in_verbosity, const char* in_format, va_list in_args)
{
	std::vfprintf(stdout, in_format, in_args);
}

inline void OVC_LOG(std::string in_category, ovc_verbosity in_verbosity, const char* in_format, ...)
{
	if (ovc_logging::logging_function && in_verbosity <= ovc_logging::verbosity)
	{
		std::string log_string;
		log_string += "[ ";
		log_string += verbosity_to_string(in_verbosity);
		log_string += " ] ";
		log_string += in_category;
		log_string += ": ";
		std::string format_string(in_format);
		log_string += format_string;
		log_string += "\n";

		va_list args;
		va_start(args, in_format);
		ovc_logging::logging_function(in_verbosity, log_string.c_str(), args);
		va_end(args);
	}
}