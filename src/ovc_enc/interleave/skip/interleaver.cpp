#include "ovc_enc/interleave/skip/interleaver.h"

skip_interleaver::skip_interleaver(ovc_interleave_config in_config)
{
	config = in_config;
}

std::vector<ovc_nal> skip_interleaver::interleave(std::vector<ovc_nal> in_nals)
{
	return in_nals;
}
