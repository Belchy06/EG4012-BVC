#pragma once

#include <stdint.h>

#include "ovc_common/bitstream/bitstream.h"

class ovc_entropy_encoder
{
public:
	ovc_entropy_encoder()
		: bitstream(new ovc_bitstream()){};

	virtual void encode(const uint8_t* in_bytes, size_t in_size) = 0;
	virtual void flush(uint8_t** out_bytes, size_t* out_size) = 0;

protected:
	ovc_bitstream* bitstream;
};