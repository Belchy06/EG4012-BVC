
#pragma once

#include <memory>

#include "bvc_common/util/matrix.h"

class bvc_partitioner
{
public:
	virtual std::vector<matrix<double>> partition(const matrix<double>& in_x, size_t in_num_levels) = 0;
};