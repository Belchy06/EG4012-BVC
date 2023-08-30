#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "ovc_common/log/log.h"
#include "ovc_dec/interleave/deinterleaver_factory.h"
#include "ovc_enc/interleave/interleaver_factory.h"

#include "interleave_test.h"

#define LogInterleaveTest "LogInterleaveTest"

bool interleave_test::test(ovc_interleave in_interleave, uint16_t in_seed, size_t in_raw_size)
{
	// Construct raw data container
	size_t	 raw_size = in_raw_size / 8;
	uint8_t* raw_data = new uint8_t[raw_size];

	// Populate raw data
	for (size_t i = 0; i < raw_size; i++)
	{
		raw_data[i] = (uint8_t)i;
	}

	// Print raw data info
	std::string raw_string = "";
	for (size_t i = 0; i < raw_size; i++)
	{
		std::bitset<8> x(raw_data[i]);
		raw_string += x.to_string();
		raw_string += " ";
	}
	OVC_LOG(LogInterleaveTest, OVC_VERBOSITY_DETAILS, "Raw data: [ %s ]", raw_string.c_str());

	ovc_interleave_config config;
	config.seed = in_seed;

	uint8_t* interleaved_data = new uint8_t();
	size_t	 interleaved_size = 0;
	interleave(in_interleave, config, raw_data, raw_size, &interleaved_data, &interleaved_size);

	// Print interleaved data info
	std::string interleaved_string = "";
	for (size_t i = 0; i < interleaved_size; i++)
	{
		std::bitset<8> x(interleaved_data[i]);
		interleaved_string += x.to_string();
		interleaved_string += " ";
	}
	OVC_LOG(LogInterleaveTest, OVC_VERBOSITY_DETAILS, "Interleaved data: [ %s ]", interleaved_string.c_str());

	// Decode
	uint8_t* deinterleaved_data = new uint8_t();
	size_t	 deinterleaved_size = 0;
	deinterleave(in_interleave, config, interleaved_data, interleaved_size, &deinterleaved_data, &deinterleaved_size);

	// Print deinterleaved data info
	std::string deinterleaved_string = "";
	for (size_t i = 0; i < deinterleaved_size; i++)
	{
		std::bitset<8> x(deinterleaved_data[i]);
		deinterleaved_string += x.to_string();
		deinterleaved_string += " ";
	}
	OVC_LOG(LogInterleaveTest, OVC_VERBOSITY_DETAILS, "Deinterleaved data: [ %s ]", deinterleaved_string.c_str());

	bool success = true;
	success &= (deinterleaved_size == raw_size);
	if (success)
	{
		for (size_t i = 0; i < raw_size; i++)
		{
			success &= (deinterleaved_data[i] == raw_data[i]);
		}
	}

	OVC_LOG(LogInterleaveTest, OVC_VERBOSITY_VERBOSE, "%s", (success ? "Success" : "Fail"));

	return success;
}

void interleave_test::interleave(ovc_interleave in_interleave, ovc_interleave_config in_config, uint8_t* in_raw_data, size_t in_raw_size, uint8_t** out_interleaved_data, size_t* out_interleaved_size)
{
	std::shared_ptr<ovc_interleaver> interleaver = ovc_interleaver_factory::create_interleaver(in_interleave, in_config);

	interleaver->interleave(in_raw_data, in_raw_size);

	// extract interleaved info
	interleaver->flush(out_interleaved_data, out_interleaved_size);
}

void interleave_test::deinterleave(ovc_interleave in_interleave, ovc_interleave_config in_config, uint8_t* in_interleaved_data, size_t in_interleaved_size, uint8_t** out_deinterleaved_data, size_t* out_deinterleaved_size)
{
	std::shared_ptr<ovc_deinterleaver> deinterleaver = ovc_deinterleaver_factory::create_deinterleaver(in_interleave, in_config);

	deinterleaver->deinterleave(in_interleaved_data, in_interleaved_size);

	// extract deinterleaved info
	deinterleaver->flush(out_deinterleaved_data, out_deinterleaved_size);
}