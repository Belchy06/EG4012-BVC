#pragma once

#include <vector>

#include "ovc_common/bitstream/bitstream.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_common/nal.h"

class ovc_interleaver
{
public:
	virtual std::vector<ovc_nal> interleave(std::vector<ovc_nal> in_nals) = 0;

protected:
	ovc_interleave_config config;
};