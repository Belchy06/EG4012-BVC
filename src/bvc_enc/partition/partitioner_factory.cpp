#include "bvc_enc/partition/offset_zerotree/partitioner.h"
#include "bvc_enc/partition/zerotree_preserving/partitioner.h"

#include "bvc_enc/partition/partitioner_factory.h"

std::shared_ptr<bvc_partitioner> bvc_partitioner_factory::create_partitioner(bvc_partition in_partition)
{
	switch (in_partition)
	{
		case bvc_partition::BVC_PARTITION_OFFSET_ZEROTREE:
			return std::make_shared<offset_zerotree_partitioner>();
		case bvc_partition::BVC_PARTITION_ZEROTREE_PRESERVING:
			return std::make_shared<zerotree_preserving_partitioner>();
		default:
			return nullptr;
	}
}