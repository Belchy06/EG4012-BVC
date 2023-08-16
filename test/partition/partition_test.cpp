#include <cmath>

#include "bvc_common/log.h"
#include "partition_test.h"

#define LogPartitionTest "LogPartitionTest"

bool partition_test::test(size_t in_x, size_t in_y)
{
	matrix<double> mat(in_x, in_y);
	mat.rand(0);

	std::vector<matrix<double>> streams = partition_test::partition(mat, 5);

	return true;
}

std::vector<matrix<double>> partition_test::partition(matrix<double>& in_matrix, size_t in_num_levels)
{
	std::vector<matrix<double>> streams;
	double						num_streams = (in_matrix.get_num_rows() * in_matrix.get_num_columns()) / pow(4, in_num_levels);
	size_t						raw_x = in_matrix.get_num_columns();
	size_t						raw_y = in_matrix.get_num_rows();

	// smallest size, split into num_streams, that's step size
	for (size_t i = 0; i < (size_t)num_streams; i++)
	{
		matrix<double> stream = matrix<double>(in_matrix.get_num_rows() / num_streams, in_matrix.get_num_columns() / num_streams);

		size_t y = 0;
		while (y < stream.get_num_rows())
		{
			size_t x = 0;
			while (x < stream.get_num_columns())
			{
				for (int l = in_num_levels - 1; n >= 0; n--)
				{
					// How big is this level?
					size_t mat_x = raw_x >> l;
					size_t mat_y = raw_y >> l;
					// How many coefficients should we be taking from this level?
					size_t x_samples_per_level = mat_x / (size_t)num_streams;
					size_t y_samples_per_level = mat_y / (size_t)num_streams;
					// How many coefficients should we be stepping over from one to the next?
					size_t step_x = mat_x / x_samples_per_level;
					size_t step_y = mat_y / y_samples_per_level;

					for (size_t level_y = 0; level_y < mat_y; level_y += step_x)
					{
						for (size_t x = 0; x < mat_x; x += step_x)
						{
						}
					}

					LOG(LogPartitionTest, BVC_VERBOSITY_VERBOSE, "{}, {}", mat_x, mat_y);
					LOG(LogPartitionTest, BVC_VERBOSITY_VERBOSE, "{}, {}", x_samples_per_level, y_samples_per_level);
				}
			}
		}

		streams.push_back(stream);
	}

	matrix<double> full;
	for (int n = in_num_levels - 1; n >= 0; n--)
	{
		size_t mat_x = raw_x >> n;
		size_t mat_y = raw_y >> n;
		size_t x_samples_per_level = mat_x / (size_t)num_streams;
		size_t y_samples_per_level = mat_y / (size_t)num_streams;

		size_t step_x = mat_x / x_samples_per_level;
		size_t step_y = mat_y / y_samples_per_level;
		for (size_t x = 0; x < mat_x; x += step_x)
		{
		}

		LOG(LogPartitionTest, BVC_VERBOSITY_VERBOSE, "{}, {}", mat_x, mat_y);
		LOG(LogPartitionTest, BVC_VERBOSITY_VERBOSE, "{}, {}", x_samples_per_level, y_samples_per_level);
	}

	return streams;
}