#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "ovc_common/log.h"
#include "ovc_dec/entropy/decoder_factory.h"
#include "ovc_enc/entropy/encoder_factory.h"

#include "entropy_test.h"

#define LogEntropyTest "LogEntropyTest"

bool entropy_test::test(ovc_entropy_coder in_entropy_coder, size_t in_raw_size)
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
	std::string raw_string = "";
	for (size_t i = 0; i < raw_size; i++)
	{
		std::bitset<8> x(raw_data[i]);
		raw_string += x.to_string();
		raw_string += " ";
	}
	LOG(LogEntropyTest, OVC_VERBOSITY_VERY_VERBOSE, "Raw data: [ {} ]", raw_string);

	uint8_t* coded_data = new uint8_t();
	size_t	 coded_size = 0;
	encode(in_entropy_coder, raw_data, raw_size, &coded_data, &coded_size);

	// Print coded data info
	std::string coded_string = "";
	for (size_t i = 0; i < coded_size; i++)
	{
		std::bitset<8> x(coded_data[i]);
		coded_string += x.to_string();
		coded_string += " ";
	}
	LOG(LogEntropyTest, OVC_VERBOSITY_VERY_VERBOSE, "Coded data: [ {} ]", coded_string);

	// Decode
	uint8_t* decoded_data = new uint8_t();
	size_t	 decoded_size = 0;
	decode(in_entropy_coder, coded_data, coded_size, raw_size, &decoded_data, &decoded_size);

	// Print coded data info
	std::string decoded_string = "";
	for (size_t i = 0; i < decoded_size; i++)
	{
		std::bitset<8> x(decoded_data[i]);
		decoded_string += x.to_string();
		decoded_string += " ";
	}
	LOG(LogEntropyTest, OVC_VERBOSITY_VERY_VERBOSE, "Decoded data: [ {} ]", decoded_string);

	bool success = true;
	success &= (decoded_size == raw_size);
	if (success)
	{
		for (size_t i = 0; i < raw_size; i++)
		{
			success &= (decoded_data[i] == raw_data[i]);
		}
	}

	LOG(LogEntropyTest, OVC_VERBOSITY_VERBOSE, "Raw size: {} bytes", raw_size);
	LOG(LogEntropyTest, OVC_VERBOSITY_VERBOSE, "Compressed size: {} bytes", coded_size);
	LOG(LogEntropyTest, OVC_VERBOSITY_VERBOSE, "{}", (success ? "Success" : "Fail"));

	return success;
}

void entropy_test::encode(ovc_entropy_coder in_entropy_coder, uint8_t* in_raw_data, size_t in_raw_size, uint8_t** out_coded_data, size_t* out_coded_size)
{
	std::shared_ptr<ovc_entropy_encoder> entropy_encoder = ovc_entropy_encoder_factory::create_entropy_encoder(in_entropy_coder);

	entropy_encoder->encode(in_raw_data, in_raw_size);

	// extract encoded info
	entropy_encoder->flush(out_coded_data, out_coded_size);
}

void entropy_test::decode(ovc_entropy_coder in_entropy_coder, uint8_t* in_coded_data, size_t in_coded_size, size_t in_raw_size, uint8_t** out_decoded_data, size_t* out_decoded_size)
{
	std::shared_ptr<ovc_entropy_decoder> entropy_decoder = ovc_entropy_decoder_factory::create_entropy_decoder(in_entropy_coder);

	// decode, informing the decoder of the data, the number of bytes of raw data, and the number of symbols (bits) we wish to extract
	entropy_decoder->decode(in_coded_data, in_coded_size, in_raw_size << 3);

	// extract encoded info
	entropy_decoder->flush(out_decoded_data, out_decoded_size);
}