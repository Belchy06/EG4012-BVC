#pragma once

#include <memory>

#include "ovc_dec/entropy/decoder.h"
#include "ovc_common/entropy/entropy.h"

class ovc_entropy_decoder_factory
{
public:
	static std::shared_ptr<ovc_entropy_decoder> create_entropy_decoder(ovc_entropy_coder in_coder);
};