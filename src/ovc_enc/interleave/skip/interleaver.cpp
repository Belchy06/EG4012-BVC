#include "ovc_enc/interleave/skip/interleaver.h"

skip_interleaver::skip_interleaver(ovc_interleave_config in_config)
{
	config = in_config;
}

void skip_interleaver::interleave(const uint8_t* in_bytes, size_t in_size)
{
	for (size_t i = 0; i < in_size; i++)
	{
		bytes.push_back(in_bytes[i]);
	}
}

void skip_interleaver::flush(uint8_t** out_bytes, size_t* out_size)
{
	*out_size = bytes.size();
	*out_bytes = std::move(bytes.data());

	bytes.clear();
}
