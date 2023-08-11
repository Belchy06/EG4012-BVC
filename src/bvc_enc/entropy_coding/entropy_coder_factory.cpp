#include "entropy_coder_factory.h"
#include "cabac/cabac.h"

std::shared_ptr<bvc_entropy_coder> bvc_entropy_coder_factory::create_entropy_coder(bvc_entropy in_coder)
{
	switch (in_coder)
	{
		case bvc_entropy::BVC_ENTROPY_CODER_CABAC:
			return std::make_shared<cabac>();
		case bvc_entropy::BVC_ENTROPY_CODER_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}