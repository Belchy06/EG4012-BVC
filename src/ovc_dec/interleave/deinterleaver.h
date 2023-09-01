#pragma once

#include <vector>

#include "ovc_common/bitstream/bitstream.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_common/nal.h"

class deinterleaver
{
public:
	virtual std::vector<ovc::nal> deinterleave(std::vector<ovc::nal> in_nals) = 0;

protected:
	ovc::interleave_config config;
};