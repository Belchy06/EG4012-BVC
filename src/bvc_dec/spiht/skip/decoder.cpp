#include "bvc_dec/spiht/skip/decoder.h"

void skip_spiht_decoder::decode(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, bvc_spiht_config in_config)
{
	output = matrix<double>(in_y, in_x);
	size_t byte_idx = 0;

	for (size_t y = 0; y < in_y; y++)
	{
		for (size_t x = 0; x < in_x; x++)
		{
			double value;
			memcpy(&value, in_bytes + byte_idx, sizeof(value));
			byte_idx += 8;

			output(y, x) = value;
		}
	}
}

void skip_spiht_decoder::flush(matrix<double>& out_matrix)
{
	out_matrix = output;
}