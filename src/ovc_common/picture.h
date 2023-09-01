#pragma once

#include <stdint.h>

#include "ovc_common/format.h"
#include "ovc_common/plane.h"

namespace ovc
{
	typedef struct picture
	{
		ovc::plane		   planes[3];
		ovc::chroma_format format;
		double			   framerate;
	} picture;
} // namespace ovc
