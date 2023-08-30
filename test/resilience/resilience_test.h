#pragma once

#include <string>

class resilience_test
{
public:
	static bool test(size_t in_num_dropped_nals, uint16_t in_seed, std::string in_output_path);
};