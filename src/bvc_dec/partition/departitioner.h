
#pragma once

#include <memory>

#include "bvc_common/math/matrix.h"

class bvc_departitioner
{
public:
	virtual matrix<double> departition(std::vector<matrix<double>>& in_streams, size_t in_num_levels) = 0;
};