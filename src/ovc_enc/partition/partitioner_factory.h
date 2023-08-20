
#pragma once

#include <memory>

#include "ovc_common/partition/partition.h"
#include "ovc_enc/partition/partitioner.h"

class ovc_partitioner_factory
{
public:
	static std::shared_ptr<ovc_partitioner> create_partitioner(ovc_partition in_partition);
};