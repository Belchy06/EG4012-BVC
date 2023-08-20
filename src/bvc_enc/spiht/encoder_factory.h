#pragma once

#include <memory>

#include "bvc_enc/spiht/encoder.h"

class bvc_spiht_encoder_factory
{
public:
	static std::shared_ptr<bvc_spiht_encoder> create_spiht_encoder(bvc_spiht in_spiht);
};