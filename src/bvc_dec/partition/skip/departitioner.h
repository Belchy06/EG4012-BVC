#pragma once

#include "bvc_dec/partition/departitioner.h"

class skip_departitioner : public bvc_departitioner
{
	virtual matrix<double> departition(std::vector<matrix<double>>& in_streams, size_t in_num_levels) override;
};