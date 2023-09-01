#include <map>

#include "ovc_dec/interleave/random/deinterleaver.h"

random_deinterleaver::random_deinterleaver(ovc::interleave_config in_config)
{
	config = in_config;
	generator = std::mt19937(config.seed);
}

std::vector<ovc::nal> random_deinterleaver::deinterleave(std::vector<ovc::nal> in_nals)
{
	std::vector<size_t> indices;
	for (size_t i = 0; i < in_nals.size(); i++)
	{
		indices.push_back(i);
	}

	std::shuffle(indices.begin(), indices.end(), generator);

	std::map<size_t, ovc::nal> nal_map;
	for (size_t i = 0; i < in_nals.size(); i++)
	{
		nal_map[indices[i]] = in_nals[i];
	}

	std::vector<ovc::nal> out_nals;
	for (auto& nal : nal_map)
	{
		out_nals.push_back(nal.second);
	}

	return out_nals;
}