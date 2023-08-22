#pragma once

#include <stdint.h>

#include "ovc_common/format.h"
#include "ovc_common/plane.h"

class ovc_picture
{
public:
	ovc_plane		  planes[3];
	ovc_chroma_format format;
	double			  framerate;
};