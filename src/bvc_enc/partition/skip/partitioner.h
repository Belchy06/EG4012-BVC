#pragma once

#include "bvc_enc/partition/partitioner.h"

class skip_partitioner : public bvc_partitioner
{
public:
	virtual std::vector<matrix<double>> partition(const matrix<double>& in_x, size_t in_num_levels) override;
};