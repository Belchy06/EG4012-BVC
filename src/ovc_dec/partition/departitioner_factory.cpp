#include "ovc_dec/partition/offset_zerotree/departitioner.h"
#include "ovc_dec/partition/skip/departitioner.h"
#include "ovc_dec/partition/zerotree_preserving/departitioner.h"

#include "ovc_dec/partition/departitioner_factory.h"

std::shared_ptr<departitioner> departitioner_factory::create_departitioner(ovc::partition in_partition)
{
	switch (in_partition)
	{
		case ovc::PARTITION_SKIP:
			return std::make_shared<skip_departitioner>();
		case ovc::PARTITION_OFFSET_ZEROTREE:
			return std::make_shared<offset_zerotree_departitioner>();
		case ovc::PARTITION_ZEROTREE_PRESERVING:
			return std::make_shared<zerotree_preserving_departitioner>();
		default:
			return nullptr;
	}
}