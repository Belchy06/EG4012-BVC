#include "ovc_dec/interleave/skip/deinterleaver.h"

skip_deinterleaver::skip_deinterleaver(ovc::interleave_config in_config)
{
	config = in_config;
}

std::vector<ovc::nal> skip_deinterleaver::deinterleave(std::vector<ovc::nal> in_nals)
{
	return in_nals;
}