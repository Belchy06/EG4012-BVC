#include <algorithm>

#include "ovc_enc/interleave/random/interleaver.h"

random_interleaver::random_interleaver(ovc_interleave_config in_config)
{
	config = in_config;
	generator = std::mt19937(config.seed);
}

std::vector<ovc_nal> random_interleaver::interleave(std::vector<ovc_nal> in_nals)
{
	std::vector<size_t> indices;
	for (size_t i = 0; i < in_nals.size(); i++)
	{
		indices.push_back(i);
	}

	std::shuffle(indices.begin(), indices.end(), generator);

	std::vector<ovc_nal> out_nals;
	for (size_t i = 0; i < in_nals.size(); i++)
	{
		out_nals.push_back(in_nals[indices[i]]);
	}

	return out_nals;
}