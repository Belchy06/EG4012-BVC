#pragma once

#include "ovc_dec/interleave/deinterleaver.h"

class skip_deinterleaver : public ovc_deinterleaver
{
public:
	skip_deinterleaver(ovc_interleave_config in_config);

	virtual std::vector<ovc_nal> deinterleave(std::vector<ovc_nal> in_nals) override;
};