#pragma once

#include <stdint.h>

#include "info.h"

class bvc_decoded_picture
{
public:
	uint8_t*	   bytes;
	size_t		   size;
	bvc_image_info info;
};