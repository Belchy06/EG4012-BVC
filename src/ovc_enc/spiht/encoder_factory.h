#pragma once

#include <memory>

#include "ovc_enc/spiht/encoder.h"

class ovc_spiht_encoder_factory
{
public:
	static std::shared_ptr<ovc_spiht_encoder> create_spiht_encoder(ovc_spiht in_spiht);
};