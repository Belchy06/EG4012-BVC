#include "entropy_decoder_factory.h"
#include "abac/abac.h"

std::shared_ptr<bvc_entropy_decoder> bvc_entropy_decoder_factory::create_entropy_coder(bvc_entropy in_coder)
{
	switch (in_coder)
	{
		case bvc_entropy::BVC_ENTROPY_CODER_CABAC:
			return std::make_shared<abac>();
		case bvc_entropy::BVC_ENTROPY_CODER_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}