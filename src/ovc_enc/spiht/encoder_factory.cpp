#include "ovc_enc/spiht/encoder_factory.h"
#include "ovc_enc/spiht/skip/encoder.h"
#include "ovc_enc/spiht/spiht/encoder.h"

std::shared_ptr<ovc_spiht_encoder> ovc_spiht_encoder_factory::create_spiht_encoder(ovc_spiht in_spiht)
{
	switch (in_spiht)
	{
		case OVC_SPIHT_SKIP:
			return std::make_shared<skip_spiht_encoder>();
		case OVC_SPIHT_ENABLE:
			return std::make_shared<spiht_encoder>();
		default:
			return nullptr;
	}
}