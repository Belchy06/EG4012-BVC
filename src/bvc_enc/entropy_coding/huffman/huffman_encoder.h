#pragma once

#include "bvc_common/bitstream.h"
#include "../entropy_encoder.h"

class huffman_encoder : public bvc_entropy_encoder
{
public:
	huffman_encoder();

	virtual void encode(const uint8_t* in_bytes, size_t in_size) override;

	virtual void flush(uint8_t** out_bits, size_t* out_size) override;

private:
	void encode_internal(uint8_t in_symbol);

private:
};