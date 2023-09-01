#pragma once

#include <stdint.h>

#include "ovc_common/bitstream/bitstream.h"

class entropy_encoder
{
public:
	entropy_encoder()
		: bitstream(new ovc::bitstream()){};

	virtual void encode(const uint8_t* in_bytes, size_t in_size) = 0;
	virtual void flush(uint8_t** out_bytes, size_t* out_size) = 0;

protected:
	ovc::bitstream* bitstream;
};