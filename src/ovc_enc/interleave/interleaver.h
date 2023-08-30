#pragma once

#include <vector>

#include "ovc_common/bitstream/bitstream.h"
#include "ovc_common/interleave/interleave_config.h"

class ovc_interleaver
{
public:
	virtual void interleave(const uint8_t* in_bytes, size_t in_size) = 0;
	virtual void flush(uint8_t** out_bytes, size_t* out_size) = 0;

protected:
	std::vector<uint8_t>  bytes;
	ovc_interleave_config config;
};