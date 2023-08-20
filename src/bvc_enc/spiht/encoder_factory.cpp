#include "bvc_enc/spiht/encoder_factory.h"
#include "bvc_enc/spiht/skip/encoder.h"
#include "bvc_enc/spiht/spiht/encoder.h"

std::shared_ptr<bvc_spiht_encoder> bvc_spiht_encoder_factory::create_spiht_encoder(bvc_spiht in_spiht)
{
	switch (in_spiht)
	{
		case BVC_SPIHT_SKIP:
			return std::make_shared<skip_spiht_encoder>();
		case BVC_SPIHT_ENABLE:
			return std::make_shared<spiht_encoder>();
		default:
			return nullptr;
	}
}