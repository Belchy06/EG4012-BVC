#pragma once

#include <memory>

#include "ovc_dec/spiht/decoder.h"

class ovc_spiht_decoder_factory
{
public:
	static std::shared_ptr<ovc_spiht_decoder> create_spiht_decoder(ovc_spiht in_spiht);
};
