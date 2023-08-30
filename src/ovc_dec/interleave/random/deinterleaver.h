#pragma once

#include <random>

#include "ovc_dec/interleave/deinterleaver.h"

class random_deinterleaver : public ovc_deinterleaver
{
public:
	random_deinterleaver(ovc_interleave_config in_config);

	virtual void deinterleave(const uint8_t* in_bytes, size_t in_size) override;
	virtual void flush(uint8_t** out_bytes, size_t* out_size) override;

private:
	std::mt19937 generator;
};