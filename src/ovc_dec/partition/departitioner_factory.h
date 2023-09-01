
#pragma once

#include <memory>

#include "ovc_common/partition/partition.h"
#include "ovc_dec/partition/departitioner.h"

class departitioner_factory
{
public:
	static std::shared_ptr<departitioner> create_departitioner(ovc::partition in_partition);
};