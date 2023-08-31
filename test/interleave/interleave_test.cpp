#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "ovc_common/log/log.h"
#include "ovc_dec/interleave/deinterleaver_factory.h"
#include "ovc_enc/interleave/interleaver_factory.h"

#include "interleave_test.h"

#define LogInterleaveTest "LogInterleaveTest"

bool interleave_test::test(ovc_interleave in_interleave, uint16_t in_seed, size_t in_num_nals)
{
	// Construct raw data container
	std::vector<ovc_nal> raw_nals;

	// Populate raw data
	for (size_t i = 0; i < in_num_nals; i++)
	{
		ovc_nal nal;
		nal.size = i;
		raw_nals.push_back(nal);
	}

	// Interleave
	ovc_interleave_config config;
	config.seed = in_seed;

	std::vector<ovc_nal> interleaved_nals;
	interleave(in_interleave, config, raw_nals, interleaved_nals);

	// Deinterleave
	std::vector<ovc_nal> deinterleaved_nals;
	deinterleave(in_interleave, config, interleaved_nals, deinterleaved_nals);

	// Print deinterleaved data info

	bool success = true;
	success &= (deinterleaved_nals.size() == raw_nals.size());
	if (success)
	{
		for (size_t i = 0; i < raw_nals.size(); i++)
		{
			success &= (raw_nals[i].size == deinterleaved_nals[i].size);
		}
	}

	OVC_LOG(LogInterleaveTest, OVC_VERBOSITY_VERBOSE, "%s", (success ? "Success" : "Fail"));

	return success;
}

void interleave_test::interleave(ovc_interleave in_interleave, ovc_interleave_config in_config, std::vector<ovc_nal> in_nals, std::vector<ovc_nal>& out_nals)
{
	std::shared_ptr<ovc_interleaver> interleaver = ovc_interleaver_factory::create_interleaver(in_interleave, in_config);

	out_nals = interleaver->interleave(in_nals);
}

void interleave_test::deinterleave(ovc_interleave in_interleave, ovc_interleave_config in_config, std::vector<ovc_nal> in_nals, std::vector<ovc_nal>& out_nals)
{
	std::shared_ptr<ovc_deinterleaver> deinterleaver = ovc_deinterleaver_factory::create_deinterleaver(in_interleave, in_config);

	out_nals = deinterleaver->deinterleave(in_nals);
}