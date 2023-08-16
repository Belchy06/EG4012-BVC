
#pragma once

#include <memory>

#include "bvc_common/partition/partition.h"
#include "bvc_enc/partition/partitioner.h"

class bvc_partitioner_factory
{
public:
	static std::shared_ptr<bvc_partitioner> create_partitioner(bvc_partition in_partition);
};