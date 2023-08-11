#pragma once

#include <stdint.h>

#include "bvc_common/entropy.h"
#include "bvc_enc/entropy_coding/entropy_encoder.h"
#include "bvc_dec/entropy_coding/entropy_decoder.h"

class entropy_test
{
public:
	static bool test(bvc_entropy in_entropy, size_t in_raw_size);
};