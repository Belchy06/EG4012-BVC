#pragma once

#include "bvc_common/bitstream.h"
#include "../entropy_decoder.h"

class abac_decoder : public bvc_entropy_decoder
{
public:
	abac_decoder();

	virtual void decode(uint8_t in_symbol) override;

	virtual void flush(uint8_t** out_bits, uint32_t* out_size) override;

private:
	bool decode_internal(uint8_t in_symbol, bool in_skip_update);
	void clear();
	void update();

private:
	bool	  skip_update;
	uint8_t	  bits_needed;
	uint32_t  encoded;
	uint32_t* history;

	uint32_t low;
	uint32_t mid;
	uint32_t high;

	uint32_t entropy_precision;
	uint32_t entropy_precision_max;
	uint32_t entropy_half_range;
	uint32_t entropy_qtr_range;
	uint32_t entropy_3qtr_range;

	uint32_t loop_count = 0;
};