#pragma once

#include <stdint.h>

#include "format.h"

class BvcEncConfig
{
public:
	size_t			Width;
	size_t			Height;
	BvcChromaFormat Format;
};