#pragma once

#include "bvc_common/bitstream.h"
#include "../entropy_decoder.h"

class abac_decoder : public bvc_entropy_decoder
{
public:
	abac_decoder();

	virtual void decode(uint8_t* in_bytes, size_t in_size, size_t in_num_symbols) override;

	virtual void flush(uint8_t** out_bits, size_t* out_size) override;

private:
	void decode_internal();
	void clear();
	void update();

private:
	uint32_t encoded;
	uint32_t history[2];

	uint32_t low;
	uint32_t mid;
	uint32_t high;

	uint32_t entropy_precision;
	uint32_t entropy_precision_max;
	uint32_t entropy_half_range;
	uint32_t entropy_qtr_range;
	uint32_t entropy_3qtr_range;
};