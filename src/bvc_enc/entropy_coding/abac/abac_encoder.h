#pragma once

#include "bvc_common/bitstream.h"
#include "../entropy_encoder.h"

class abac_encoder : public bvc_entropy_encoder
{
public:
	abac_encoder();

	virtual void encode_symbol(uint8_t in_symbol) override;

	virtual void flush(uint8_t** out_bits, uint32_t* out_size) override;

private:
	void clear();
	void update();

	void flush_inverse_bits(uint8_t in_symbol);

private:
	uint32_t* history;
	uint32_t  encoded;
	uint32_t  underflow_count;

	uint32_t low;
	uint32_t mid;
	uint32_t high;

	uint32_t entropy_precision;
	uint32_t entropy_precision_max;
	uint32_t entropy_half_range;
	uint32_t entropy_qtr_range;
	uint32_t entropy_3qtr_range;
	uint64_t entropy_msb_mask;
};