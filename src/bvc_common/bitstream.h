#pragma once

#include <stdint.h>
#include <vector>

class bitstream
{
public:
	bitstream();

	void write_bit(uint8_t in_bit);
	void write_byte(uint8_t in_byte);

	void read_bit(uint8_t* out_bit);
	void read_byte(uint8_t* out_byte);

	uint8_t* data();
	uint32_t size();	  // Return the size (in bytes)
	uint32_t occupancy(); // Return the occupied space (in bytes) (rounding up if needed)

private:
	void	 clear();
	uint32_t align(uint32_t in_value, uint32_t in_alignment);

private:
	uint32_t			 read_idx;	// Bit
	uint32_t			 write_idx; // Bit
	uint32_t			 capacity;	// Bytes
	std::vector<uint8_t> stream;
};