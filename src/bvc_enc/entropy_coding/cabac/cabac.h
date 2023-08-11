#pragma once

#include "bvc_common/bitstream.h"
#include "context.h"
#include "../entropy_coder.h"

class cabac : public bvc_entropy_encoder
{
public:
	cabac();

	virtual void encode_symbol(uint8_t in_symbol) override;

	virtual void flush(uint8_t* out_bits, uint32_t* out_size) override;

private:
	void clear();
	void update();
	void encode_symbol(uint8_t in_symbol, context* in_ctx);

	void flush_inverse_bits(uint8_t in_symbol);

private:
	// TODO (belchy06): Move this to a class
	uint32_t model;

	uint32_t* history;
	uint32_t  value;
	uint32_t  e3_count;

	uint32_t low;
	uint32_t mid;
	uint32_t high;
};