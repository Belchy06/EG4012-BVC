#include "ovc_enc/interleave/interleaver_factory.h"

#include "ovc_enc/interleave/skip/interleaver.h"
#include "ovc_enc/interleave/random/interleaver.h"

std::shared_ptr<ovc_interleaver> ovc_interleaver_factory::create_interleaver(ovc_interleave in_interleave, ovc_interleave_config in_config)
{
	switch (in_interleave)
	{
		case OVC_INTERLEAVE_SKIP:
			return std::make_shared<skip_interleaver>(in_config);
		case OVC_INTERLEAVE_RANDOM:
			return std::make_shared<random_interleaver>(in_config);
		default:
			return nullptr;
	}
}