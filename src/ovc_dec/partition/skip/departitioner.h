#pragma once

#include "ovc_dec/partition/departitioner.h"

class skip_departitioner : public ovc_departitioner
{
	virtual matrix<double> departition(std::vector<matrix<double>>& in_streams, size_t in_num_levels) override;
};