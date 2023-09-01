#pragma once

#include "ovc_dec/partition/departitioner.h"

class zerotree_preserving_departitioner : public departitioner
{
public:
	zerotree_preserving_departitioner();

	virtual matrix<double> departition(std::vector<matrix<double>>& in_partitions, size_t in_num_levels) override;
};
