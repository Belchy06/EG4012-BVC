#pragma once

#include <cstdarg>
#include <format>
#include <functional>
#include <iostream>
#include <string>

#include "ovc_common/verbosity.h"

namespace ovc
{
	typedef void (*logging_callback)(int, const char*, va_list);

	namespace logging
	{
		inline verbosity									  verbosity;
		extern std::function<void(int, const char*, va_list)> logging_function;
	} // namespace logging

	inline void DEFAULT_LOG(int in_verbosity, const char* in_format, va_list in_args)
	{
		std::vfprintf(stdout, in_format, in_args);
	}

	inline void LOG(std::string in_category, verbosity in_verbosity, const char* in_format, ...)
	{
		if (logging::logging_function && in_verbosity <= logging::verbosity)
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
			logging::logging_function(in_verbosity, log_string.c_str(), args);
			va_end(args);
		}
	}
} // namespace ovc
