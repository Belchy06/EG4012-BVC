#pragma once

#include <stdint.h>

#include "bvc_common/bitstream/bitstream.h"

class bvc_entropy_encoder
{
public:
	bvc_entropy_encoder()
		: stream(new bitstream()){};

	virtual void encode(const uint8_t* in_bytes, size_t in_size) = 0;

	virtual void flush(uint8_t** out_bits, size_t* out_size) = 0;

protected:
	bitstream* stream;
};