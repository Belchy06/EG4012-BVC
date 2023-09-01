#include "ovc_dec/interleave/deinterleaver_factory.h"

#include "ovc_dec/interleave/skip/deinterleaver.h"
#include "ovc_dec/interleave/random/deinterleaver.h"

std::shared_ptr<deinterleaver> deinterleaver_factory::create_deinterleaver(ovc::interleave in_interleave, ovc::interleave_config in_config)
{
	switch (in_interleave)
	{
		case ovc::INTERLEAVE_SKIP:
			return std::make_shared<skip_deinterleaver>(in_config);
		case ovc::INTERLEAVE_RANDOM:
			return std::make_shared<random_deinterleaver>(in_config);
		default:
			return nullptr;
	}
}