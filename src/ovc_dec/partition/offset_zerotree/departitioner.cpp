#include <cmath>

#include "ovc_common/log/log.h"
#include "ovc_dec/partition/offset_zerotree/departitioner.h"

#define LogOffsetZerotreeDepartitioner "LogOffsetZerotreeDepartitioner"

offset_zerotree_departitioner::offset_zerotree_departitioner()
{
}

matrix<double> offset_zerotree_departitioner::departition(std::vector<matrix<double>>& in_partitions, size_t in_num_levels)
{
	size_t num_partitions = in_partitions.size();

	matrix<double> full((size_t)sqrt(num_partitions) * in_partitions[0].get_num_rows(), (size_t)sqrt(num_partitions) * in_partitions[0].get_num_columns());

	for (size_t i = 0; i < num_partitions; i++)
	{
		matrix<double> partition = in_partitions[i];

		// How big is this level?
		size_t mat_x = (size_t)sqrt(num_partitions) * partition.get_num_columns();
		size_t mat_y = (size_t)sqrt(num_partitions) * partition.get_num_rows();
		// How many coefficients should we be taking from this level?
		size_t x_samples_per_stream = (size_t)sqrt(num_partitions) * mat_x / (size_t)num_partitions;
		size_t y_samples_per_stream = (size_t)sqrt(num_partitions) * mat_y / (size_t)num_partitions;
		// How many coefficients should we be stepping over from one to the next?
		size_t step_x = (size_t)(num_partitions / (size_t)sqrt(num_partitions));
		size_t step_y = (size_t)(num_partitions / (size_t)sqrt(num_partitions));

		for (size_t y = 0; y < y_samples_per_stream; y++)
		{
			for (size_t x = 0; x < x_samples_per_stream; x++)
			{
				size_t offset_x = i % step_x;
				size_t offset_y = i / step_y;
				full(y * step_y + offset_y, x * step_x + offset_x) = partition(y, x);
			}
		}
	}

	return full;
}
