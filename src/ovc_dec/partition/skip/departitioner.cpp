#include "ovc_dec/partition/skip/departitioner.h"

matrix<double> skip_departitioner::departition(std::vector<matrix<double>>& in_partitions, size_t in_num_levels)
{
	return in_partitions[0];
}