#include "bvc_dec/partition/offset_zerotree/departitioner.h"
#include "bvc_dec/partition/skip/departitioner.h"
#include "bvc_dec/partition/zerotree_preserving/departitioner.h"

#include "bvc_dec/partition/departitioner_factory.h"

std::shared_ptr<bvc_departitioner> bvc_departitioner_factory::create_departitioner(bvc_partition in_partition)
{
	switch (in_partition)
	{
		case BVC_PARTITION_SKIP:
			return std::make_shared<skip_departitioner>();
		case BVC_PARTITION_OFFSET_ZEROTREE:
			return std::make_shared<offset_zerotree_departitioner>();
		case BVC_PARTITION_ZEROTREE_PRESERVING:
			return std::make_shared<zerotree_preserving_departitioner>();
		default:
			return nullptr;
	}
}