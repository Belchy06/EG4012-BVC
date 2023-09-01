#pragma once

#include <memory>

#include "ovc_common/interleave/interleave.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_dec/interleave/deinterleaver.h"

class deinterleaver_factory
{
public:
	static std::shared_ptr<deinterleaver> create_deinterleaver(ovc::interleave in_interleave, ovc::interleave_config in_config);
};