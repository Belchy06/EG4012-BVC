#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "bvc_dec/entropy_coding/entropy_decoder.h"
#include "bvc_dec/entropy_coding/entropy_decoder_factory.h"
#include "bvc_enc/entropy_coding/entropy_encoder.h"
#include "bvc_enc/entropy_coding/entropy_encoder_factory.h"

#include "entropy_test.h"

bool entropy_test::test(bvc_entropy_coder in_entropy_coder, size_t in_raw_size, bvc_verbosity in_verbosity)
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
	if (in_verbosity >= BVC_VERBOSITY_VERY_VERBOSE)
	{
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
	}

	uint8_t* coded_data = new uint8_t();
	size_t	 coded_size = 0;
	encode(in_entropy_coder, raw_data, raw_size, &coded_data, &coded_size);

	// Print coded data info
	if (in_verbosity >= BVC_VERBOSITY_VERY_VERBOSE)
	{
		std::cout << "Compressed Data: " << std::endl;
		std::cout << "[ ";
		for (size_t i = 0; i < coded_size; i++)
		{
			std::bitset<8> x(coded_data[i]);
			std::cout << x << " ";
		}
		std::cout << "] " << std::endl;
		std::cout << std::endl;
	}

	// Decode
	uint8_t* decoded_data = new uint8_t();
	size_t	 decoded_size = 0;
	decode(in_entropy_coder, coded_data, coded_size, raw_size, &decoded_data, &decoded_size);

	// Print coded data info
	if (in_verbosity > BVC_VERBOSITY_VERBOSE)
	{
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
	}

	bool success = true;
	success &= (decoded_size == raw_size);
	if (success)
	{
		for (size_t i = 0; i < raw_size; i++)
		{
			success &= (decoded_data[i] == raw_data[i]);
		}
	}

	if (in_verbosity >= BVC_VERBOSITY_VERBOSE)
	{
		std::cout << "Raw size: " << raw_size << "bytes" << std::endl;
		std::cout << "Compressed size: " << coded_size << "bytes" << std::endl;
		std::cout << (success ? "Success" : "Fail") << std::endl;
	}

	return success;
}

void entropy_test::encode(bvc_entropy_coder in_entropy_coder, uint8_t* in_raw_data, size_t in_raw_size, uint8_t** out_coded_data, size_t* out_coded_size)
{
	std::shared_ptr<bvc_entropy_encoder> entropy_encoder = bvc_entropy_encoder_factory::create_entropy_encoder(in_entropy_coder);

	entropy_encoder->encode(in_raw_data, in_raw_size);

	// extract encoded info
	entropy_encoder->flush(out_coded_data, out_coded_size);
}

void entropy_test::decode(bvc_entropy_coder in_entropy_coder, uint8_t* in_coded_data, size_t in_coded_size, size_t in_raw_size, uint8_t** out_decoded_data, size_t* out_decoded_size)
{
	std::shared_ptr<bvc_entropy_decoder> entropy_decoder = bvc_entropy_decoder_factory::create_entropy_decoder(in_entropy_coder);

	// decode, informing the decoder of the data, the number of bytes of raw data, and the number of symbols (bits) we wish to extract
	entropy_decoder->decode(in_coded_data, in_coded_size, in_raw_size << 3);

	// extract encoded info
	entropy_decoder->flush(out_decoded_data, out_decoded_size);
}