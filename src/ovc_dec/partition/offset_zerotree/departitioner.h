#pragma once

#include "ovc_dec/partition/departitioner.h"

class offset_zerotree_departitioner : public ovc_departitioner
{
public:
	offset_zerotree_departitioner();

	virtual matrix<double> departition(std::vector<matrix<double>>& in_streams, size_t in_num_levels) override;
};
