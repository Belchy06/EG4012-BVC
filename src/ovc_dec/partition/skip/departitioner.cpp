#include "ovc_dec/partition/skip/departitioner.h"

matrix<double> skip_departitioner::departition(std::vector<matrix<double>>& in_streams, size_t in_num_levels)
{
	return in_streams[0];
}