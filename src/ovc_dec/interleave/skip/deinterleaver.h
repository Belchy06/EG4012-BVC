#pragma once

#include "ovc_dec/interleave/deinterleaver.h"

class skip_deinterleaver : public deinterleaver
{
public:
	skip_deinterleaver(ovc::interleave_config in_config);

	virtual std::vector<ovc::nal> deinterleave(std::vector<ovc::nal> in_nals) override;
};