#pragma once

#include "bvc_common/bitstream.h"
#include "../entropy_encoder.h"

class abac : public bvc_entropy_encoder
{
public:
	abac();

	virtual void encode_symbol(uint8_t in_symbol) override;

	virtual void flush(uint8_t** out_bits, uint32_t* out_size) override;

private:
	void clear();
	void update();

	void flush_inverse_bits(uint8_t in_symbol);

private:
	uint32_t* history;
	uint32_t  value;
	uint32_t  e3_count;

	uint32_t low;
	uint32_t mid;
	uint32_t high;
};