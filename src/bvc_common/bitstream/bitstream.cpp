#include <memory>
#include <stdint.h>

#include <iostream>
#include "bvc_common/bitstream/bitstream.h"

bitstream::bitstream()
	: read_idx(0)
	, write_idx(0)
	, capacity(0)
{
	stream.resize(capacity);
}

void bitstream::write_bit(uint8_t in_bit)
{
	in_bit = in_bit & 0x1;

	if (write_idx + 1 > stream.size() << 3)
	{
		// Add a new empty element to the end of the vector
		stream.push_back(0);
	}

	uint32_t dest_byte = write_idx >> 3;
	uint8_t	 dest_bit = write_idx % 8;

	uint8_t* byte_data = &(stream[dest_byte]);
	*byte_data = ((*byte_data) & ~(0x1 << dest_bit)) | (in_bit & 0x1) << dest_bit;

	write_idx++;
	return;
}

void bitstream::write_byte(uint8_t in_byte)
{
	if (write_idx + 8 > stream.size() << 3)
	{
		// Add a new empty element to the end of the vector
		stream.push_back(0);
	}

	/* Determine the current byte to write. */
	uint32_t dest_byte = write_idx >> 3;
	uint8_t	 dest_bit = write_idx % 8;

	if (dest_bit == 0)
	{
		/* This is a byte aligned write, so we perform it at byte level. */
		uint8_t* data = &(stream[dest_byte]);
		*data = in_byte;
		write_idx += 8;
	}
	else
	{
		/* Slower byte unaligned write. */
		for (uint8_t i = 0; i < 8; ++i)
		{
			write_bit((in_byte >> i) & 0x1);
		}
	}
}

bool bitstream::read_bit(uint8_t* out_bit)
{
	if (read_idx >= write_idx)
	{
		return false;
	}

	/* Determine the current byte to read from. */
	uint32_t src_byte = read_idx >> 3;
	uint8_t	 src_bit = read_idx % 8;

	/* Pull the correct byte from our stream store. Note that we
	   preserve the high bits of *dest. */
	*out_bit &= 0xFE;
	uint8_t bit = ((stream[src_byte]) >> src_bit) & 0x1;
	*out_bit |= bit;
	read_idx++;

	return true;
}

bool bitstream::read_byte(uint8_t* out_byte)
{
	return true;
}

uint8_t* bitstream::data()
{
	return stream.data();
}

uint32_t bitstream::size()
{
	return write_idx - read_idx;
}

uint32_t bitstream::occupancy()
{
	return align(write_idx - read_idx, 8) >> 3;
}

void bitstream::clear()
{
	write_idx = 0;
	read_idx = 0;

	stream.clear();
	stream.resize(0);
	capacity = 0;
}

uint32_t bitstream::align(uint32_t in_value, uint32_t in_alignment)
{
	uint32_t mod = in_value % in_alignment;

	if (mod != 0)
	{
		in_value += in_alignment - mod;
	}

	return in_value;
}
