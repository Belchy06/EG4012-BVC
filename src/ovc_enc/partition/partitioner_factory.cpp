#include "ovc_enc/partition/offset_zerotree/partitioner.h"
#include "ovc_enc/partition/skip/partitioner.h"
#include "ovc_enc/partition/zerotree_preserving/partitioner.h"

#include "ovc_enc/partition/partitioner_factory.h"

std::shared_ptr<partitioner> partitioner_factory::create_partitioner(ovc::partition in_partition)
{
	switch (in_partition)
	{
		case ovc::PARTITION_SKIP:
			return std::make_shared<skip_partitioner>();
		case ovc::PARTITION_OFFSET_ZEROTREE:
			return std::make_shared<offset_zerotree_partitioner>();
		case ovc::PARTITION_ZEROTREE_PRESERVING:
			return std::make_shared<zerotree_preserving_partitioner>();
		default:
			return nullptr;
	}
}