#pragma once

#include <stdint.h>

#include "bvc_common/format.h"

class BvcDecConfig
{
public:
	size_t			Width;
	size_t			Height;
	BvcChromaFormat Format;
};