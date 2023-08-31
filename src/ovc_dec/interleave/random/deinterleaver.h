#pragma once

#include <random>

#include "ovc_dec/interleave/deinterleaver.h"

class random_deinterleaver : public ovc_deinterleaver
{
public:
	random_deinterleaver(ovc_interleave_config in_config);

	virtual std::vector<ovc_nal> deinterleave(std::vector<ovc_nal> in_nals) override;

private:
	std::mt19937 generator;
};