#include <cstring>

#include "huffman_encoder.h"

huffman_encoder::huffman_encoder()
{
	stream = new bitstream();
}

void huffman_encoder::encode(const uint8_t* in_bytes, size_t in_size)
{
	for (size_t i = 0; i < in_size; i++)
	{
		encode_internal(in_bytes[i]);
	}
}

void huffman_encoder::flush(uint8_t** out_bits, size_t* out_size)
{
	*out_bits = new uint8_t[stream->occupancy()];
	memcpy(*out_bits, stream->data(), stream->occupancy());
	*out_size = stream->occupancy();
}

void huffman_encoder::encode_internal(uint8_t in_symbol)
{
}