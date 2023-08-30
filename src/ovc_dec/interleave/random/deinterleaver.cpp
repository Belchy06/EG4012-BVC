#include "ovc_dec/interleave/random/deinterleaver.h"

random_deinterleaver::random_deinterleaver(ovc_interleave_config in_config)
{
	config = in_config;
	generator = std::mt19937(config.seed);
}

void random_deinterleaver::deinterleave(const uint8_t* in_bytes, size_t in_size)
{
	std::vector<size_t> indices;
	for (size_t i = 0; i < in_size; i++)
	{
		indices.push_back(i);
	}

	std::shuffle(indices.begin(), indices.end(), generator);

	bytes.clear();
	bytes.assign(in_size, 0);
	for (size_t i = 0; i < in_size; i++)
	{
		bytes[indices[i]] = in_bytes[i];
	}
}

void random_deinterleaver::flush(uint8_t** out_bytes, size_t* out_size)
{
	*out_size = bytes.size();
	memcpy(*out_bytes, bytes.data(), bytes.size());
}
