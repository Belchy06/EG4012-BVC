#include <cmath>

#include "ovc_common/log/log.h"
#include "ovc_enc/partition/offset_zerotree/partitioner.h"

#define LogOffsetZerotreePartitioner "LogOffsetZerotreePartitioner"

offset_zerotree_partitioner::offset_zerotree_partitioner()
{
}

std::vector<matrix<double>> offset_zerotree_partitioner::partition(const matrix<double>& in_matrix, size_t in_num_levels, size_t in_num_parts)
{
	std::vector<matrix<double>> partitions;
	size_t						raw_x = in_matrix.get_num_columns();
	size_t						raw_y = in_matrix.get_num_rows();

	// smallest size, split into in_num_parts, that's step size
	for (size_t i = 0; i < (size_t)in_num_parts; i++)
	{
		size_t		   partition_height = (size_t)(sqrt(in_num_parts) * in_matrix.get_num_rows() / in_num_parts);
		size_t		   partition_width = (size_t)(sqrt(in_num_parts) * in_matrix.get_num_columns() / in_num_parts);
		matrix<double> partition = matrix<double>(partition_height, partition_width);

		// How big is this level?
		size_t mat_x = raw_x;
		size_t mat_y = raw_y;
		// How many coefficients should we be taking from this level?
		size_t x_samples_per_stream = (size_t)sqrt(in_num_parts) * mat_x / (size_t)in_num_parts;
		size_t y_samples_per_stream = (size_t)sqrt(in_num_parts) * mat_y / (size_t)in_num_parts;
		// How many coefficients should we be stepping over from one to the next?
		size_t step_x = mat_x / x_samples_per_stream;
		size_t step_y = mat_y / y_samples_per_stream;

		for (size_t y = 0; y < y_samples_per_stream; y++)
		{
			for (size_t x = 0; x < x_samples_per_stream; x++)
			{
				size_t offset_x = i % step_x;
				size_t offset_y = i / step_y;
				partition(y, x) = in_matrix(y * step_y + offset_y, x * step_x + offset_x);
			}
		}

		partitions.push_back(partition);
	}

	return partitions;
}
