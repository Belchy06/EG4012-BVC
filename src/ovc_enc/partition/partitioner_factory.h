
#pragma once

#include <memory>

#include "ovc_common/partition/partition.h"
#include "ovc_enc/partition/partitioner.h"

class partitioner_factory
{
public:
	static std::shared_ptr<partitioner> create_partitioner(ovc::partition in_partition);
};