#pragma once

#include <string>

#include "ovc_common/verbosity.h"

typedef enum
{
	OVC_ERROR_CONCEALMENT_SKIP,
	OVC_ERROR_CONCEALMENT_AVERAGE_RECEIVED,
	OVC_ERROR_CONCEALMENT_AVERAGE_SURROUNDING
} ovc_error_concealment;

inline std::string error_concealment_to_string(ovc_error_concealment in_error_concealment)
{
	switch (in_error_concealment)
	{
		case OVC_ERROR_CONCEALMENT_SKIP:
			return "OVC_ERROR_CONCEALMENT_SKIP";
		case OVC_ERROR_CONCEALMENT_AVERAGE_RECEIVED:
			return "OVC_ERROR_CONCEALMENT_AVERAGE_RECEIVED";
		case OVC_ERROR_CONCEALMENT_AVERAGE_SURROUNDING:
			return "OVC_ERROR_CONCEALMENT_AVERAGE_SURROUNDING";
		default:
			return "";
	}
}

class ovc_dec_config
{
public:
	ovc_verbosity		  log_verbosity;
	ovc_error_concealment error_concealment;
};