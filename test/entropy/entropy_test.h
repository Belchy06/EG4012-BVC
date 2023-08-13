#pragma once

#include <stdint.h>

#include "bvc_common/entropy.h"
#include "bvc_common/verbosity.h"

class entropy_test
{
public:
	static bool test(bvc_entropy in_entropy, size_t in_raw_size, bvc_verbosity in_verbosity);

private:
	static void encode(bvc_entropy in_entropy, uint8_t* in_raw_data, size_t in_raw_size, uint8_t** out_coded_data, size_t* out_coded_size);
	static void decode(bvc_entropy in_entropy, uint8_t* in_coded_data, size_t in_coded_size, size_t in_raw_size, uint8_t** out_decoded_data, size_t* out_decoded_size);
};