#pragma once

#include <memory>
#include <stdint.h>

#include "ovc_common/partition/partition.h"
#include "ovc_common/math/matrix.h"
#include "ovc_common/verbosity.h"

class partition_test
{
public:
	static bool test(ovc_partition in_partition, size_t in_x, size_t in_y);

private:
	static std::vector<matrix<double>> partition(ovc_partition in_partition, matrix<double>& in_matrix, size_t in_num_levels);
};