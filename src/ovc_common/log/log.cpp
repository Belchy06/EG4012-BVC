#include "ovc_common/log/log.h"

std::function<void(int, const char*, va_list)> ovc_logging::logging_function = DEFAULT_OVC_LOG;