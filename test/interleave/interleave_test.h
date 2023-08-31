#pragma once

#include <stdint.h>
#include <vector>

#include "ovc_common/interleave/interleave.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_common/verbosity.h"
#include "ovc_common/nal.h"

class interleave_test
{
public:
	static bool test(ovc_interleave in_interleave, uint16_t in_seed, size_t in_num_nals);

private:
	static void interleave(ovc_interleave in_interleave, ovc_interleave_config in_config, std::vector<ovc_nal> in_nals, std::vector<ovc_nal>& out_nals);
	static void deinterleave(ovc_interleave in_interleave, ovc_interleave_config in_config, std::vector<ovc_nal> in_nals, std::vector<ovc_nal>& out_nals);
};