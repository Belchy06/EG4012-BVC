#include <cmath>

#include "ovc_common/log/log.h"
#include "partition_test.h"

#include "ovc_enc/partition/partitioner_factory.h"

#define LogPartitionTest "LogPartitionTest"

bool partition_test::test(ovc_partition in_partition, size_t in_x, size_t in_y)
{
	matrix<double>		mat(in_x, in_y);
	std::vector<double> data;

	for (size_t i = 0; i < in_x * in_y; i++)
	{
		data.push_back((double)i);
	}

	mat.set_data(data);

	// TODO (belchy06): num levels should depend on image size?
	std::vector<matrix<double>> partitions = partition_test::partition(in_partition, mat, 5, 4);

	return true;
}

std::vector<matrix<double>> partition_test::partition(ovc_partition in_partition, matrix<double>& in_matrix, size_t in_num_levels, size_t in_num_partitions)
{
	std::shared_ptr<ovc_partitioner> partitioner = ovc_partitioner_factory::create_partitioner(in_partition);

	return partitioner->partition(in_matrix, in_num_levels, in_num_partitions);
}