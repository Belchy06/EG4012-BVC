#pragma once

#include "ovc_enc/interleave/interleaver.h"

class skip_interleaver : public ovc_interleaver
{
public:
	skip_interleaver(ovc_interleave_config in_config);

	virtual std::vector<ovc_nal> interleave(std::vector<ovc_nal> in_nals) override;
};