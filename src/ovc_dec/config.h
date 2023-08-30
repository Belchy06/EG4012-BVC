#pragma once

#include "ovc_common/verbosity.h"

typedef enum
{
	OVC_ERROR_CONCEALMENT_SKIP,
	OVC_ERROR_CONCEALMENT_AVERAGE_RECEIVED,
	OVC_ERROR_CONCEALMENT_AVERAGE_SURROUNDING
} ovc_error_concealment;

class ovc_dec_config
{
public:
	ovc_verbosity		  log_verbosity;
	ovc_error_concealment error_concealment;
};