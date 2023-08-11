#pragma once

#include <memory>

#include "entropy_coder.h"
#include "bvc_common/entropy.h"

class bvc_entropy_encoder_factory
{
public:
	static std::shared_ptr<bvc_entropy_encoder> create_entropy_encoder(bvc_entropy in_coder);
};