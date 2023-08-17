
#pragma once

#include <memory>

#include "bvc_common/partition/partition.h"
#include "bvc_dec/partition/departitioner.h"

class bvc_departitioner_factory
{
public:
	static std::shared_ptr<bvc_departitioner> create_departitioner(bvc_partition in_partition);
};