#pragma once

#include <memory>

#include "bvc_enc/entropy/encoder.h"
#include "bvc_common/entropy/entropy.h"

class bvc_entropy_encoder_factory
{
public:
	static std::shared_ptr<bvc_entropy_encoder> create_entropy_encoder(bvc_entropy_coder in_coder);
};