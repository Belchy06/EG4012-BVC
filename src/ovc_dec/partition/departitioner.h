
#pragma once

#include <memory>

#include "ovc_common/math/matrix.h"

class ovc_departitioner
{
public:
	virtual matrix<double> departition(std::vector<matrix<double>>& in_partitions, size_t in_num_levels) = 0;
};