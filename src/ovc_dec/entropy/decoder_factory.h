#pragma once

#include <memory>

#include "ovc_dec/entropy/decoder.h"
#include "ovc_common/entropy/entropy.h"

class entropy_decoder_factory
{
public:
	static std::shared_ptr<entropy_decoder> create_entropy_decoder(ovc::entropy_coder in_coder);
};