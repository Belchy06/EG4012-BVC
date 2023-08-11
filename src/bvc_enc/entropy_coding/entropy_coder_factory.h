#pragma once

#include <memory>

#include "entropy_coder.h"
#include "bvc_common/entropy.h"

class bvc_entropy_coder_factory
{
public:
	static std::shared_ptr<bvc_entropy_coder> create_entropy_coder(bvc_entropy in_coder);
};