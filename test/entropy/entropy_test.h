#pragma once

#include <stdint.h>

#include "ovc_common/entropy/entropy.h"
#include "ovc_common/verbosity.h"

class entropy_test
{
public:
	static bool test(ovc_entropy_coder in_entropy_coder, size_t in_raw_size);

private:
	static void encode(ovc_entropy_coder in_entropy_coder, uint8_t* in_raw_data, size_t in_raw_size, uint8_t** out_coded_data, size_t* out_coded_size);
	static void decode(ovc_entropy_coder in_entropy_coder, uint8_t* in_coded_data, size_t in_coded_size, size_t in_raw_size, uint8_t** out_decoded_data, size_t* out_decoded_size);
};