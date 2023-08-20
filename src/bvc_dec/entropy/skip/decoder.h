#pragma once

#include "bvc_dec/entropy/decoder.h"

class skip_entropy_decoder : public bvc_entropy_decoder
{
public:
	virtual void decode(uint8_t* in_bytes, size_t in_size, size_t in_num_symbols) override;
	virtual void flush(uint8_t** out_bits, size_t* out_size) override;
};