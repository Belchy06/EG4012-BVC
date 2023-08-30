#pragma once

#include <stdint.h>

#include "ovc_common/interleave/interleave.h"
#include "ovc_common/interleave/interleave_config.h"
#include "ovc_common/verbosity.h"

class interleave_test
{
public:
	static bool test(ovc_interleave in_interleave, uint16_t in_seed, size_t in_raw_size);

private:
	static void interleave(ovc_interleave in_interleave, ovc_interleave_config in_config, uint8_t* in_raw_data, size_t in_raw_size, uint8_t** out_interleaved_data, size_t* out_interleaved_size);
	static void deinterleave(ovc_interleave in_interleave, ovc_interleave_config in_config, uint8_t* in_interleaved_data, size_t in_interleaved_size, uint8_t** out_deinterleaved_data, size_t* out_deinterleaved_size);
};