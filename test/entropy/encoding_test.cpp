#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "bvc_dec/entropy_coding/entropy_decoder.h"
#include "bvc_dec/entropy_coding/entropy_decoder_factory.h"
#include "bvc_enc/entropy_coding/entropy_encoder.h"
#include "bvc_enc/entropy_coding/entropy_encoder_factory.h"

#include "encoding_test.h"

bool entropy_test::test(bvc_entropy in_entropy, size_t in_raw_size)
{
	// Construct raw data container
	size_t	 raw_size = in_raw_size / 8;
	uint8_t* raw_data = new uint8_t[raw_size];

	// Populate raw data
	for (size_t i = 0; i < raw_size; i++)
	{
		raw_data[i] = 1;
	}

	// Print raw data info
	std::cout << "=============================" << std::endl;
	std::cout << "Raw Data: " << std::endl;
	std::cout << "[ ";
	for (size_t i = 0; i < raw_size; i++)
	{
		std::bitset<8> x(raw_data[i]);
		std::cout << x << " ";
	}
	std::cout << "] " << std::endl;
	std::cout << std::endl;

	uint8_t* coded_data = new uint8_t();
	uint32_t coded_size;
	encode(in_entropy, raw_data, raw_size, &coded_data, &coded_size);

	// Print coded data info
	std::cout << "Compressed Data: " << std::endl;
	std::cout << "[ ";
	for (size_t i = 0; i < coded_size; i++)
	{
		std::bitset<8> x(coded_data[i]);
		std::cout << x << " ";
	}
	std::cout << "] " << std::endl;
	std::cout << std::endl;

	// Decode
	uint8_t* decoded_data = new uint8_t();
	uint32_t decoded_size;
	decode(in_entropy, coded_data, coded_size, raw_size, &decoded_data, &decoded_size);

	// Print coded data info
	std::cout << std::endl
			  << "Decoded Data: " << std::endl;
	std::cout << "[ ";
	for (size_t i = 0; i < decoded_size; i++)
	{
		std::bitset<8> x(decoded_data[i]);
		std::cout << x << " ";
	}
	std::cout << "] " << std::endl;
	std::cout << std::endl;

	return true;
}

void entropy_test::encode(bvc_entropy in_entropy, uint8_t* in_raw_data, uint32_t in_raw_size, uint8_t** out_coded_data, uint32_t* out_coded_size)
{
	std::shared_ptr<bvc_entropy_encoder> entropy_encoder = bvc_entropy_encoder_factory::create_entropy_encoder(in_entropy);

	// encode bytes, one bit at a time
	for (size_t i = 0; i < in_raw_size; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			uint8_t bit = (in_raw_data[i] >> j) & 0x1;
			entropy_encoder->encode_symbol(bit);
		}
	}

	// extract encoded info
	entropy_encoder->flush(out_coded_data, out_coded_size);
}

void entropy_test::decode(bvc_entropy in_entropy, uint8_t* in_coded_data, uint32_t in_coded_size, uint32_t in_raw_size, uint8_t** out_decoded_data, uint32_t* out_decoded_size)
{
	std::shared_ptr<bvc_entropy_decoder> entropy_decoder = bvc_entropy_decoder_factory::create_entropy_decoder(in_entropy);

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			uint8_t bit = (i < in_coded_size) ? (in_coded_data[i] >> j) & 0x1 : 0;
			entropy_decoder->decode(bit);
		}
	}

	for (size_t i = 2; i < in_raw_size; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			size_t	bitIdx = i * 8 + j;
			uint8_t bit = (i < in_coded_size) ? (in_coded_data[i] >> j) & 0x1 : 0;
			entropy_decoder->decode(bit);
		}
	}

	// extract encoded info
	entropy_decoder->flush(out_decoded_data, out_decoded_size);
}