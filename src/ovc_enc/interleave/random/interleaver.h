#pragma once

#include <random>

#include "ovc_enc/interleave/interleaver.h"

class random_interleaver : public interleaver
{
public:
	random_interleaver(ovc::interleave_config in_config);

	virtual std::vector<ovc::nal> interleave(std::vector<ovc::nal> in_nals) override;

private:
	std::mt19937 generator;
};