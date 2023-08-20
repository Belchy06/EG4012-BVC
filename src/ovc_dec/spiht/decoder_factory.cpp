#include "ovc_dec/spiht/decoder_factory.h"
#include "ovc_dec/spiht/skip/decoder.h"
#include "ovc_dec/spiht/spiht/decoder.h"

std::shared_ptr<ovc_spiht_decoder> ovc_spiht_decoder_factory::create_spiht_decoder(ovc_spiht in_spiht)
{
	switch (in_spiht)
	{
		case OVC_SPIHT_SKIP:
			return std::make_shared<skip_spiht_decoder>();
		case OVC_SPIHT_ENABLE:
			return std::make_shared<spiht_decoder>();
		default:
			return nullptr;
	}
}