#pragma once

#include <memory>
#include <stdint.h>

#include "bvc_common/verbosity.h"
#include "bvc_common/util/matrix.h"

class partition_test
{
public:
	static bool test(size_t in_x, size_t in_y);

private:
	static std::vector<matrix<double>> partition(matrix<double>& in_matrix, size_t in_num_levels);
};