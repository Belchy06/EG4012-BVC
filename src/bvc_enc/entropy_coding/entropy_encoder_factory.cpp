#include "entropy_encoder_factory.h"
#include "abac/abac.h"

std::shared_ptr<bvc_entropy_encoder> bvc_entropy_encoder_factory::create_entropy_encoder(bvc_entropy in_coder)
{
	switch (in_coder)
	{
		case bvc_entropy::BVC_ENTROPY_CODER_ABAC:
			return std::make_shared<abac>();
		case bvc_entropy::BVC_ENTROPY_CODER_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}