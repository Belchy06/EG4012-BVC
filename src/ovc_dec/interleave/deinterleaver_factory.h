#pragma once

#include <memory>

#include "ovc_common/interleave/interleave.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_dec/interleave/deinterleaver.h"

class ovc_deinterleaver_factory
{
public:
	static std::shared_ptr<ovc_deinterleaver> create_deinterleaver(ovc_interleave in_interleave, ovc_interleave_config in_config);
};