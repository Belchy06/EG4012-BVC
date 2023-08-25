#pragma once

#include "ovc_enc/partition/partitioner.h"

class zerotree_preserving_partitioner : public ovc_partitioner
{
public:
	zerotree_preserving_partitioner();

	virtual std::vector<matrix<double>> partition(const matrix<double>& in_matrix, size_t in_num_levels, size_t in_num_parts) override;
};
