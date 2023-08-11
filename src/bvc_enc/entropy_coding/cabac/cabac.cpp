#include <cstring>

#include "cabac.h"

cabac::cabac()
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

void cabac::encode_symbol(uint8_t in_symbol)
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
}

void cabac::flush(uint8_t* out_bits, uint32_t* out_size)
{
	e3_count++;

	uint8_t val = (low < ((uint32_t(0x1) << 16) - 1) >> 2) ? 0 : 1;
	stream->write_bit(val);
	flush_inverse_bits(val);

	out_bits = new uint8_t[stream->size()];
	memcpy(out_bits, stream->data(), stream->size());
	*out_size = stream->size();
}

void cabac::clear()
{
	low = 0;
	value = 0;
	e3_count = 0;

	history[0] = history[1] = 1;
	high = (uint32_t(0x1) << 16) - 1;
	mid = high >> 1;
}

void cabac::update()
{
	uint32_t range = high - low;
	uint64_t mid_range = range * history[0] / (history[0] + history[1]);

	mid = low + (uint32_t)mid_range;
}

void cabac::encode_symbol(uint8_t in_symbol, context* in_ctx)
{
}

void cabac::flush_inverse_bits(uint8_t in_symbol)
{
	in_symbol = !in_symbol;
	for (uint32_t i = 0; i < e3_count; i++)
	{
		stream->write_bit(in_symbol);
	}

	e3_count = 0;
}