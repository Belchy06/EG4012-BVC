
#pragma once

#include <memory>

#include "ovc_common/math/matrix.h"

class ovc_partitioner
{
public:
	virtual std::vector<matrix<double>> partition(const matrix<double>& in_x, size_t in_num_levels) = 0;
};