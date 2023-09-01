#pragma once

#include "ovc_enc/interleave/interleaver.h"

class skip_interleaver : public interleaver
{
public:
	skip_interleaver(ovc::interleave_config in_config);

	virtual std::vector<ovc::nal> interleave(std::vector<ovc::nal> in_nals) override;
};