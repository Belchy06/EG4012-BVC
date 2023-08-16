#pragma once

#include <memory>
#include <stdint.h>

#include "bvc_common/partition/partition.h"
#include "bvc_common/math/matrix.h"
#include "bvc_common/verbosity.h"

class partition_test
{
public:
	static bool test(bvc_partition in_partition, size_t in_x, size_t in_y);

private:
	static std::vector<matrix<double>> partition(bvc_partition in_partition, matrix<double>& in_matrix, size_t in_num_levels);
};