#pragma once

#include <memory>

#include "bvc_dec/spiht/decoder.h"

class bvc_spiht_decoder_factory
{
public:
	static std::shared_ptr<bvc_spiht_decoder> create_spiht_decoder(bvc_spiht in_spiht);
};
