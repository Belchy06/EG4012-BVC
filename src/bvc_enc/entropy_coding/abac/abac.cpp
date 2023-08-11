#include <cstring>

#include "abac.h"

abac::abac()
	: low(0)
	// 16 bit precision
	, high((uint32_t(0x1) << 16) - 1)
	, mid(high >> 1)
	, value(0)
	, e3_count(0)
{
	history = new uint32_t[]{ 1, 1 };
	stream = new bitstream();
}

void abac::encode_symbol(uint8_t in_symbol)
{
	// Zero out the 7 MSBs, leaving just the LSB
	in_symbol = in_symbol & 0b1;

	// We only encode the first 2^31 instances of a symbol
	if (history[in_symbol] >= (uint32_t)2 * (uint32_t)1024 * (uint32_t)1024 * (uint32_t)1024)
	{
		return;
	}

	update();

	// Update bounds based on new information
	if (in_symbol)
	{
		low = mid + 1;
	}
	else
	{
		high = mid;
	}

	history[in_symbol]++;

	uint32_t entropy_precision = 16;
	uint32_t entropy_precision_max = (uint32_t(0x1) << entropy_precision) - 1;
	uint32_t entropy_half_range = (entropy_precision_max >> 1);
	uint32_t entropy_qtr_range = (entropy_half_range >> 1);
	uint32_t entropy_3qtr_range = 3 * entropy_qtr_range;
	uint64_t entropy_msb_mask = (uint64_t(0x1) << (entropy_precision - 1));
	while (true)
	{
		if ((high & entropy_msb_mask) == (low & entropy_msb_mask))
		{
			/* E1/E2 scaling violation. */
			uint8_t msb = (uint8_t)((high & entropy_msb_mask) >> 15);
			low -= entropy_half_range * msb + msb;
			high -= entropy_half_range * msb + msb;

			stream->write_bit(msb);
			flush_inverse_bits(msb);
		}
		else if (high <= entropy_3qtr_range && low > entropy_qtr_range)
		{
			/* E3 scaling violation. */
			high -= entropy_qtr_range + 1;
			low -= entropy_qtr_range + 1;
			e3_count += 1;
		}
		else
		{
			break;
		}

		high = ((high << 0x1) & entropy_precision_max) | 0x1;
		low = ((low << 0x1) & entropy_precision_max) | 0x0;
	}
}

void abac::flush(uint8_t** out_bits, uint32_t* out_size)
{
	e3_count++;

	uint8_t val = (low < ((uint32_t(0x1) << 16) - 1) >> 2) ? 0 : 1;
	stream->write_bit(val);
	flush_inverse_bits(val);

	*out_bits = new uint8_t[stream->occupancy()];
	memcpy(*out_bits, stream->data(), stream->occupancy());
	*out_size = stream->occupancy();

	clear();
}

void abac::clear()
{
	low = 0;
	value = 0;
	e3_count = 0;

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

void abac::flush_inverse_bits(uint8_t in_symbol)
{
	in_symbol = !in_symbol;
	for (uint32_t i = 0; i < e3_count; i++)
	{
		stream->write_bit(in_symbol);
	}

	e3_count = 0;
}