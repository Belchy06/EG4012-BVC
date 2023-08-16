#pragma once

#include "bvc_enc/partition/partitioner.h"

class offset_zerotree_partitioner : public bvc_partitioner
{
public:
	offset_zerotree_partitioner();

	virtual std::vector<matrix<double>> partition(const matrix<double>& in_matrix, size_t in_num_levels) override;
};
