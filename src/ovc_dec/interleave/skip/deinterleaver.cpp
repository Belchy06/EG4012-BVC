#include "ovc_dec/interleave/skip/deinterleaver.h"

skip_deinterleaver::skip_deinterleaver(ovc_interleave_config in_config)
{
	config = in_config;
}

void skip_deinterleaver::deinterleave(const uint8_t* in_bytes, size_t in_size)
{
	bytes.clear();
	for (size_t i = 0; i < in_size; i++)
	{
		bytes.push_back(in_bytes[i]);
	}
}

void skip_deinterleaver::flush(uint8_t** out_bytes, size_t* out_size)
{
	*out_size = bytes.size();
	memcpy(*out_bytes, bytes.data(), bytes.size());
}
