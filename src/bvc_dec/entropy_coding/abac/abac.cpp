#include <cstring>

#include "abac.h"

abac::abac()
	: low(0)
	// 16 bit precision
	, high((uint32_t(0x1) << 16) - 1)
	, mid(high >> 1)
	, value(0)
{
	history = new uint32_t[]{ 1, 1 };
	stream = new bitstream();
}

void abac::decode_symbol(uint32_t in_symbol)
{
	update();

	if (in_symbol >= low && in_symbol < mid)
	{
		high = mid;
		history[0]++;
		stream->write_bit(0);
	}
	else if (in_symbol >= mid && in_symbol < high)
	{
		low = mid + 1;
		history[1]++;
		stream->write_bit(1);
	}

	uint32_t entropy_precision = 16;
	uint32_t entropy_precision_max = (uint32_t(0x1) << entropy_precision);
	uint32_t entropy_half_range = (entropy_precision_max >> 1);
	uint32_t entropy_qtr_range = (entropy_half_range >> 1);
	while (true)
	{

		if (high <= entropy_half_range)
		{
			/* If our high value is less than half we do nothing (but
			   prevent the loop from exiting). */
		}
		else if (low > entropy_half_range)
		{
			high -= (entropy_half_range + 1);
			low -= (entropy_half_range + 1);
			value -= (entropy_half_range + 1);
		}
		else if (high <= 3 * entropy_qtr_range && low > entropy_qtr_range)
		{
			/* E3 scaling violation. */
			high -= entropy_qtr_range + 1;
			low -= entropy_qtr_range + 1;
			value -= entropy_qtr_range + 1;
		}
		else
		{
			break;
		}

		high = ((high << 0x1) & entropy_precision_max) | 0x1;
		low = ((low << 0x1) & entropy_precision_max) | 0x0;
		value = ((value << 0x1) & entropy_precision_max) | in_symbol;
	}
}

void abac::flush(uint8_t* out_bits, uint32_t* out_size)
{
	out_bits = new uint8_t[stream->occupancy()];
	memcpy(out_bits, stream->data(), stream->occupancy());
	*out_size = stream->occupancy();
}

void abac::clear()
{
	low = 0;
	value = 0;

	history[0] = history[1] = 1;
	high = (uint32_t(0x1) << 16) - 1;
	mid = high >> 1;
}

void abac::update()
{
	uint32_t range = high - low;
	uint64_t mid_range = range * history[0] / (history[0] + history[1]);

	mid = low + (uint32_t)mid_range;
}