#include "ovc_dec/interleave/deinterleaver_factory.h"

#include "ovc_dec/interleave/skip/deinterleaver.h"
#include "ovc_dec/interleave/random/deinterleaver.h"

std::shared_ptr<ovc_deinterleaver> ovc_deinterleaver_factory::create_deinterleaver(ovc_interleave in_interleave, ovc_interleave_config in_config)
{
	switch (in_interleave)
	{
		case OVC_INTERLEAVE_SKIP:
			return std::make_shared<skip_deinterleaver>(in_config);
		case OVC_INTERLEAVE_RANDOM:
			return std::make_shared<random_deinterleaver>(in_config);
		default:
			return nullptr;
	}
}