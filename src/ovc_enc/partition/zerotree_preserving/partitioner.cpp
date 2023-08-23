#include "ovc_common/log.h"
#include "ovc_enc/partition/zerotree_preserving/partitioner.h"

#define LogZerotreePreservingPartitioner "LogZerotreePreservingPartitioner"

zerotree_preserving_partitioner::zerotree_preserving_partitioner()
{
}

std::vector<matrix<double>> zerotree_preserving_partitioner::partition(const matrix<double>& in_matrix, size_t in_num_levels, size_t in_num_streams)
{
	std::vector<matrix<double>> streams;
	size_t						raw_x = in_matrix.get_num_columns();
	size_t						raw_y = in_matrix.get_num_rows();

	// smallest size, split into in_num_streams, that's step size
	for (size_t i = 0; i < (size_t)in_num_streams; i++)
	{
		matrix<double> stream = matrix<double>((size_t)(2 * in_matrix.get_num_rows() / in_num_streams), (size_t)(2 * in_matrix.get_num_columns() / in_num_streams));

		for (int64_t l = in_num_levels - 1; l >= 0; l--)
		{
			// How big is this level?
			size_t mat_x = raw_x >> l;
			size_t mat_y = raw_y >> l;
			// How many coefficients should we be taking from this level?
			size_t x_samples_per_level = 2 * mat_x / (size_t)in_num_streams;
			size_t y_samples_per_level = 2 * mat_y / (size_t)in_num_streams;
			// How many coefficients should we be stepping over from one to the next?
			size_t step_x = mat_x / x_samples_per_level;
			size_t step_y = mat_y / y_samples_per_level;

			for (size_t y = 0; y < y_samples_per_level; y++)
			{
				for (size_t x = 0; x < x_samples_per_level; x++)
				{
					size_t offset_x = i % step_x;
					size_t offset_y = i / step_y;
					stream(x, y) = in_matrix((x + offset_x) * step_x, (y + offset_y) * step_y);
				}
			}
		}

		streams.push_back(stream);
	}

	return streams;
}
