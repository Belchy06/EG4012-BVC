#pragma once

#include <memory>

#include "ovc_common/interleave/interleave.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_enc/interleave/interleaver.h"

class interleaver_factory
{
public:
	static std::shared_ptr<interleaver> create_interleaver(ovc::interleave in_interleave, ovc::interleave_config in_config);
};