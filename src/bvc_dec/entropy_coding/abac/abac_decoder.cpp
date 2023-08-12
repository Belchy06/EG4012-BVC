#include <cstring>
#include <iostream>
#include "abac_decoder.h"

abac_decoder::abac_decoder()
	: low(0)
	// 16 bit precision
	, high((uint32_t(0x1) << 16) - 1)
	, mid(high >> 1)
	, bits_needed(16)
	, encoded(0)
	, skip_update(false)
{
	history = new uint32_t[]{ 1, 1 };
	stream = new bitstream();

	entropy_precision = 16;
	entropy_precision_max = (uint32_t(0x1) << entropy_precision) - 1;
	entropy_half_range = (entropy_precision_max >> 1);
	entropy_qtr_range = (entropy_half_range >> 1);
	entropy_3qtr_range = 3 * entropy_qtr_range;
}

void abac_decoder::decode(uint8_t in_symbol)
{
	bool decoded = false;
	// Decoding may break straight away, in which case the symbol provided will be skipped.
	// We must keep decoding until the symbol is utilised
	do
	{
		decoded = decode_internal(in_symbol, skip_update);
	}
	while (!decoded);
}

bool abac_decoder::decode_internal(uint8_t in_symbol, bool in_skip_update)
{
	if (bits_needed > 0)
	{
		// Read in the first 16 bits we encounter
		encoded <<= 0x1;
		encoded |= in_symbol;
		bits_needed--;
		return true;
	}

	if (!in_skip_update)
	{
		update();

		if (encoded >= low && encoded < mid)
		{
			high = mid;
			history[0]++;
			stream->write_bit(0);
		}
		else if (encoded >= mid && encoded < high)
		{
			low = mid + 1;
			history[1]++;
			stream->write_bit(1);
		}
	}
	else
	{
		high = ((high << 0x1) & entropy_precision_max) | 0x1;
		low = ((low << 0x1) & entropy_precision_max) | 0x0;
		encoded = ((encoded << 0x1) & entropy_precision_max) | in_symbol;
		skip_update = false;
	}

	bool utilised = false;
	while (true)
	{
		if (high <= entropy_half_range)
		{
			/* If our high value is less than half we do nothing (but
			   prevent the loop from exiting). */
		}
		else if (low > entropy_half_range)
		{
			high -= entropy_half_range + 1;
			low -= entropy_half_range + 1;
			encoded -= entropy_half_range + 1;
		}
		else if (high <= entropy_3qtr_range && low > entropy_qtr_range)
		{
			/* E3 scaling violation. */
			high -= entropy_qtr_range + 1;
			low -= entropy_qtr_range + 1;
			encoded -= entropy_qtr_range + 1;
		}
		else
		{
			break;
		}

		if (utilised)
		{
			skip_update = true;
			return true;
		}

		high = ((high << 0x1) & entropy_precision_max) | 0x1;
		low = ((low << 0x1) & entropy_precision_max) | 0x0;
		encoded = ((encoded << 0x1) & entropy_precision_max) | in_symbol;
		utilised = true;
	}

	std::cout << loop_count << ": " << encoded << std::endl;
	loop_count++;
	return utilised;
}

void abac_decoder::flush(uint8_t** out_bits, uint32_t* out_size)
{
	*out_bits = new uint8_t[stream->occupancy()];
	memcpy(*out_bits, stream->data(), stream->occupancy());
	*out_size = stream->occupancy();

	clear();
}

void abac_decoder::clear()
{
	low = 0;
	bits_needed = 16;

	history[0] = history[1] = 1;
	high = (uint32_t(0x1) << 16) - 1;
	mid = high >> 1;
}

void abac_decoder::update()
{
	uint32_t range = high - low;
	uint64_t mid_range = range * history[0] / (history[0] + history[1]);

	mid = low + (uint32_t)mid_range;
}