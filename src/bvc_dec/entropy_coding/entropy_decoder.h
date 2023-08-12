#pragma once

#include <stdint.h>

#include "bvc_common/bitstream.h"

class bvc_entropy_decoder
{
public:
	bvc_entropy_decoder() = default;

	virtual void decode(uint8_t in_symbol) = 0;

	virtual void flush(uint8_t** out_bits, uint32_t* out_size) = 0;

protected:
	bitstream* stream;
};