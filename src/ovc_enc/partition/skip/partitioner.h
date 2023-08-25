#pragma once

#include "ovc_enc/partition/partitioner.h"

class skip_partitioner : public ovc_partitioner
{
public:
	virtual std::vector<matrix<double>> partition(const matrix<double>& in_x, size_t in_num_levels, size_t in_num_parts) override;
};