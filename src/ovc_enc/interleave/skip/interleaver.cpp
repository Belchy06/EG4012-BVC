#include "ovc_enc/interleave/skip/interleaver.h"

skip_interleaver::skip_interleaver(ovc::interleave_config in_config)
{
	config = in_config;
}

std::vector<ovc::nal> skip_interleaver::interleave(std::vector<ovc::nal> in_nals)
{
	return in_nals;
}
