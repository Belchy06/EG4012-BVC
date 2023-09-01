#pragma once

#include <random>

#include "ovc_dec/interleave/deinterleaver.h"

class random_deinterleaver : public deinterleaver
{
public:
	random_deinterleaver(ovc::interleave_config in_config);

	virtual std::vector<ovc::nal> deinterleave(std::vector<ovc::nal> in_nals) override;

private:
	std::mt19937 generator;
};