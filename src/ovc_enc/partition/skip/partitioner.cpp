#include "ovc_enc/partition/skip/partitioner.h"

std::vector<matrix<double>> skip_partitioner::partition(const matrix<double>& in_x, size_t in_num_levels, size_t in_num_parts)
{
	std::vector<matrix<double>> partitions;
	partitions.push_back(in_x);
	return partitions;
}