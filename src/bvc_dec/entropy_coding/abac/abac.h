#pragma once

#include "bvc_common/bitstream.h"
#include "../entropy_decoder.h"

class abac : public bvc_entropy_decoder
{
public:
	abac();

	virtual void decode_symbol(uint32_t in_symbol) override;

	virtual void flush(uint8_t* out_bits, uint32_t* out_size) override;

private:
	void clear();
	void update();

private:
	uint32_t* history;
	uint32_t  value;

	uint32_t low;
	uint32_t mid;
	uint32_t high;
};