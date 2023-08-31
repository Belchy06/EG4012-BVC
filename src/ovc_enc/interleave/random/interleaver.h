#pragma once

#include <random>

#include "ovc_enc/interleave/interleaver.h"

class random_interleaver : public ovc_interleaver
{
public:
	random_interleaver(ovc_interleave_config in_config);

	virtual std::vector<ovc_nal> interleave(std::vector<ovc_nal> in_nals) override;

private:
	std::mt19937 generator;
};