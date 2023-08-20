#pragma once

#include <stdint.h>

#include "ovc_common/picture_info.h"

class ovc_picture
{
public:
	uint8_t*		 Y;
	uint8_t*		 U;
	uint8_t*		 V;
	ovc_picture_info info;
};