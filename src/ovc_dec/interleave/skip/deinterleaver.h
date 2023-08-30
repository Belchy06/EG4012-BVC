#pragma once

#include "ovc_dec/interleave/deinterleaver.h"

class skip_deinterleaver : public ovc_deinterleaver
{
public:
	skip_deinterleaver(ovc_interleave_config in_config);

	virtual void deinterleave(const uint8_t* in_bytes, size_t in_size) override;
	virtual void flush(uint8_t** out_bytes, size_t* out_size) override;
};