#include "bvc_dec/spiht/decoder_factory.h"
#include "bvc_dec/spiht/skip/decoder.h"
#include "bvc_dec/spiht/spiht/decoder.h"

std::shared_ptr<bvc_spiht_decoder> bvc_spiht_decoder_factory::create_spiht_decoder(bvc_spiht in_spiht)
{
	switch (in_spiht)
	{
		case BVC_SPIHT_SKIP:
			return std::make_shared<skip_spiht_decoder>();
		case BVC_SPIHT_ENABLE:
			return std::make_shared<spiht_decoder>();
		default:
			return nullptr;
	}
}