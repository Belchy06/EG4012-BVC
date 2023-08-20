#pragma once

#include <memory>

#include "ovc_enc/entropy/encoder.h"
#include "ovc_common/entropy/entropy.h"

class ovc_entropy_encoder_factory
{
public:
	static std::shared_ptr<ovc_entropy_encoder> create_entropy_encoder(ovc_entropy_coder in_coder);
};