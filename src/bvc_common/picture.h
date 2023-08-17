#pragma once

#include <stdint.h>

#include "bvc_common/picture_info.h"

class bvc_picture
{
public:
	uint8_t*		 Y;
	uint8_t*		 U;
	uint8_t*		 V;
	bvc_picture_info info;
};