#pragma once

#include <memory>
#include <stdint.h>

#include <iostream>
#include "bitstream.h"

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
}

void bitstream::read_bit(uint8_t* out_bit)
{
	if (read_idx >= write_idx)
	{
		return;
	}

	/* Determine the current byte to read from. */
	uint32_t src_byte = read_idx >> 3;
	uint8_t	 src_bit = read_idx % 8;
	uint8_t* dest = stream.data();

	/* Pull the correct byte from our stream store. Note that we
	   preserve the high bits of *dest. */
	*dest &= 0xFE;
	*dest |= ((stream[src_byte]) >> src_bit) & 0x1;
	read_idx++;

	return;
}

void bitstream::read_byte(uint8_t* out_byte)
{
}

uint8_t* bitstream::data()
{
	return stream.data();
}

uint32_t bitstream::size()
{
	return capacity;
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
