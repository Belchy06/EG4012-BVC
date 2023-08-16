#pragma once

#include <queue>
#include <stdint.h>

#include "bvc_common/bitstream/bitstream.h"

class bvc_entropy_decoder
{
public:
	bvc_entropy_decoder()
		: stream(new bitstream()) {}

	// Each implementation is responisble for handling decoding
	virtual void decode(uint8_t* in_bytes, size_t in_size, size_t in_num_symbols) = 0;

	// Each implementation is responisble for the retrieval of decoded data
	virtual void flush(uint8_t** out_bits, size_t* out_size) = 0;

private:
	// Each implementation is responsible for cleaning up after a decode, ready for new data
	virtual void clear() = 0;

protected:
	bitstream* stream;

	std::queue<uint8_t> bits;
};