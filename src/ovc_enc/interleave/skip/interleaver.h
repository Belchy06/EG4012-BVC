#pragma once

#include "ovc_enc/interleave/interleaver.h"

class skip_interleaver : public ovc_interleaver
{
public:
	skip_interleaver(ovc_interleave_config in_config);

	virtual void interleave(const uint8_t* in_bytes, size_t in_size) override;
	virtual void flush(uint8_t** out_bytes, size_t* out_size) override;
};