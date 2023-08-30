#pragma once

#include <memory>

#include "ovc_common/interleave/interleave.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_enc/interleave/interleaver.h"

class ovc_interleaver_factory
{
public:
	static std::shared_ptr<ovc_interleaver> create_interleaver(ovc_interleave in_interleave, ovc_interleave_config in_config);
};