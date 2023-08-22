#include <cmath>

#include "ovc_common/log.h"
#include "ovc_dec/partition/offset_zerotree/departitioner.h"

#define LogOffsetZerotreeDepartitioner "LogOffsetZerotreeDepartitioner"

offset_zerotree_departitioner::offset_zerotree_departitioner()
{
}

matrix<double> offset_zerotree_departitioner::departition(std::vector<matrix<double>>& in_streams, size_t in_num_levels)
{
	size_t num_streams = in_streams.size();

	matrix<double> full((size_t)sqrt(num_streams) * in_streams[0].get_num_rows(), (size_t)sqrt(num_streams) * in_streams[0].get_num_columns());

	for (size_t i = 0; i < num_streams; i++)
	{
		matrix<double> stream = in_streams[i];

		// How big is this level?
		size_t mat_x = (size_t)sqrt(num_streams) * stream.get_num_columns();
		size_t mat_y = (size_t)sqrt(num_streams) * stream.get_num_rows();
		// How many coefficients should we be taking from this level?
		size_t x_samples_per_stream = (size_t)sqrt(num_streams) * mat_x / (size_t)num_streams;
		size_t y_samples_per_stream = (size_t)sqrt(num_streams) * mat_y / (size_t)num_streams;
		// How many coefficients should we be stepping over from one to the next?
		size_t step_x = (size_t)(num_streams / (size_t)sqrt(num_streams));
		size_t step_y = (size_t)(num_streams / (size_t)sqrt(num_streams));

		for (size_t y = 0; y < y_samples_per_stream; y++)
		{
			for (size_t x = 0; x < x_samples_per_stream; x++)
			{
				size_t offset_x = i % step_x;
				size_t offset_y = i / step_y;
				full(y * step_y + offset_y, x * step_x + offset_x) = stream(y, x);
				// LOG(LogOffsetZerotreePartitioner, OVC_VERBOSITY_VERY_VERBOSE, "downsampled {}, {}", x, y);
				// LOG(LogOffsetZerotreePartitioner, OVC_VERBOSITY_VERY_VERBOSE, "original {}, {}", x * step_x + offset_x, y * step_y + offset_y);
			}
		}
	}

	return full;
}
