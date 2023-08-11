#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "bvc_enc/entropy_coding/entropy_encoder_factory.h"
#include "encoding_test.h"

bool entropy_test::test(bvc_entropy in_entropy, size_t in_raw_size)
{
	std::shared_ptr<bvc_entropy_encoder> entropy_coder = bvc_entropy_encoder_factory::create_entropy_encoder(in_entropy);

	// Construct raw data container
	size_t	 raw_data_bytes_size = in_raw_size / 8;
	uint8_t* raw_data = new uint8_t[raw_data_bytes_size];

	// Populate raw data
	for (size_t i = 0; i < raw_data_bytes_size; i++)
	{
		raw_data[i] = 1;
	}

	// Print raw data info
	std::cout << "=============================" << std::endl;
	std::cout << "Raw Data: " << std::endl;
	std::cout << "[ ";
	for (size_t i = 0; i < raw_data_bytes_size; i++)
	{
		std::bitset<8> x(raw_data[i]);
		std::cout << x << " ";
	}
	std::cout << "] " << std::endl;
	std::cout << std::endl;

	// Compress bytes, one bit at a time
	for (size_t i = 0; i < raw_data_bytes_size; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			uint8_t bit = (raw_data[i] >> j) & 0x1;
			entropy_coder->encode_symbol(bit);
		}
	}

	// Extract coded info
	uint8_t* compressed_data = new uint8_t();
	uint32_t compressed_size;
	entropy_coder->flush(&compressed_data, &compressed_size);

	// Print coded data info
	std::cout << "Compressed Data: " << std::endl;
	std::cout << "[ ";
	for (size_t i = 0; i < compressed_size; i++)
	{
		std::bitset<8> x(compressed_data[i]);
		std::cout << x << " ";
	}
	std::cout << "] " << std::endl;
	std::cout << std::endl;

	return true;
}