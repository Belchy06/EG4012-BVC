#pragma once

#include <queue>
#include <stdint.h>

#include "bvc_common/bitstream.h"

class bvc_entropy_decoder
{
public:
	bvc_entropy_decoder() = default;

	virtual void decode(uint8_t* in_bytes, size_t in_size, size_t in_num_symbols) = 0;

	virtual void flush(uint8_t** out_bits, size_t* out_size) = 0;

protected:
	bitstream* stream;

	std::queue<uint8_t> bits;
};