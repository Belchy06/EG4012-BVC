#pragma once

#include <stdint.h>

class bitstream
{
public:
	bitstream();

	void write_bit(uint8_t in_bit);
	void write_byte(uint8_t in_byte);

	void read_bit(uint8_t* out_bit);
	void read_byte(uint8_t* out_byte);

	uint8_t* data();
	// Return the size (in bytes) of the bit stream
	uint32_t size();

	uint32_t resize(uint32_t in_bit_capacity);

private:
	void	 clear();
	uint32_t align(uint32_t in_value, uint32_t in_alignment);

private:
	uint32_t read_idx;
	uint32_t write_idx;
	uint32_t capacity;
	uint8_t* stream;
};