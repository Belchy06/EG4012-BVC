#include "bvc_common/log.h"
#include "bvc_enc/partition/offset_zerotree/partitioner.h"

#define LogOffsetZerotreePartitioner "LogOffsetZerotreePartitioner"

offset_zerotree_partitioner::offset_zerotree_partitioner()
{
}

std::vector<matrix<double>> offset_zerotree_partitioner::partition(const matrix<double>& in_matrix, size_t in_num_levels)
{
	std::vector<matrix<double>> streams;
	double						num_streams = (in_matrix.get_num_rows() * in_matrix.get_num_columns()) / pow(4, in_num_levels);
	size_t						raw_x = in_matrix.get_num_columns();
	size_t						raw_y = in_matrix.get_num_rows();

	// smallest size, split into num_streams, that's step size
	for (size_t i = 0; i < (size_t)num_streams; i++)
	{
		matrix<double> stream = matrix<double>((size_t)(2 * in_matrix.get_num_rows() / num_streams), (size_t)(2 * in_matrix.get_num_columns() / num_streams));

		for (int l = in_num_levels - 1; l >= 0; l--)
		{
			// How big is this level?
			size_t mat_x = raw_x >> l;
			size_t mat_y = raw_y >> l;
			// How many coefficients should we be taking from this level?
			size_t x_samples_per_level = 2 * mat_x / (size_t)num_streams;
			size_t y_samples_per_level = 2 * mat_y / (size_t)num_streams;
			// How many coefficients should we be stepping over from one to the next?
			size_t step_x = mat_x / x_samples_per_level;
			size_t step_y = mat_y / y_samples_per_level;

			for (size_t y = 0; y < y_samples_per_level; y++)
			{
				for (size_t x = 0; x < x_samples_per_level; x++)
				{
					size_t offset_x = i % step_x;
					size_t offset_y = i / step_y;
					stream(y, x) = in_matrix(y * step_y + offset_y, x * step_x + offset_x);
					LOG(LogOffsetZerotreePartitioner, BVC_VERBOSITY_VERY_VERBOSE, "downsampled {}, {}", x, y);
					LOG(LogOffsetZerotreePartitioner, BVC_VERBOSITY_VERY_VERBOSE, "original {}, {}", x * step_x + offset_x, y * step_y + offset_y);
				}
			}
		}

		streams.push_back(stream);
	}

	return streams;
}
