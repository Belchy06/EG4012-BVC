
#pragma once

#include <memory>

#include "ovc_common/partition/partition.h"
#include "ovc_dec/partition/departitioner.h"

class ovc_departitioner_factory
{
public:
	static std::shared_ptr<ovc_departitioner> create_departitioner(ovc_partition in_partition);
};