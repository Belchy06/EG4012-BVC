#include <algorithm>

#include "ovc_enc/interleave/random/interleaver.h"

random_interleaver::random_interleaver(ovc_interleave_config in_config)
{
	config = in_config;
	generator = std::mt19937(config.seed);
}

void random_interleaver::interleave(const uint8_t* in_bytes, size_t in_size)
{
	std::vector<size_t> indices;
	for (size_t i = 0; i < in_size; i++)
	{
		indices.push_back(i);
	}

	std::shuffle(indices.begin(), indices.end(), generator);

	bytes.assign(in_size, 0);
	for (size_t i = 0; i < in_size; i++)
	{
		bytes[i] = in_bytes[indices[i]];
	}
}

void random_interleaver::flush(uint8_t** out_bytes, size_t* out_size)
{
	*out_size = bytes.size();
	*out_bytes = std::move(bytes.data());

	bytes.clear();
}
