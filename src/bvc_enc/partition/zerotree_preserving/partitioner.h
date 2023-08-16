#pragma once

#include "bvc_enc/partition/partitioner.h"

class zerotree_preserving_partitioner : public bvc_partitioner
{
public:
	zerotree_preserving_partitioner();

	virtual std::vector<matrix<double>> partition(const matrix<double>& in_matrix, size_t in_num_levels) override;
};
