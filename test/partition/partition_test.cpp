#include <cmath>

#include "bvc_common/log.h"
#include "partition_test.h"

#define LogPartitionTest "LogPartitionTest"

bool partition_test::test(size_t in_x, size_t in_y)
{
	matrix<double>		mat(in_x, in_y);
	std::vector<double> data;

	for (size_t i = 0; i < in_x * in_y; i++)
	{
		data.push_back(i);
	}

	mat.set_data(data);

	std::cout << mat << std::endl;

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
		matrix<double> stream = matrix<double>((size_t)(in_matrix.get_num_rows() / num_streams), (size_t)(in_matrix.get_num_columns() / num_streams));

		for (int l = in_num_levels - 1; l >= 0; l--)
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

			for (size_t y = 0; y * step_y < mat_y; y++)
			{
				for (size_t x = 0; x * step_x < mat_x; x++)
				{
					stream(x, y) = in_matrix(x * step_x + i, y * step_y + i);
					LOG(LogPartitionTest, BVC_VERBOSITY_VERBOSE, "downsampled {}, {}", x, y);
					LOG(LogPartitionTest, BVC_VERBOSITY_VERBOSE, "original {}, {}", x * step_x, y * step_y);
				}
			}
		}

		std::cout << stream << std::endl;
		streams.push_back(stream);
	}

	return streams;
}