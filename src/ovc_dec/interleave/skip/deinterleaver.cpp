#include "ovc_dec/interleave/skip/deinterleaver.h"

skip_deinterleaver::skip_deinterleaver(ovc_interleave_config in_config)
{
	config = in_config;
}

std::vector<ovc_nal> skip_deinterleaver::deinterleave(std::vector<ovc_nal> in_nals)
{
	return in_nals;
}