#pragma once

#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_enc/wavelet/decomposer.h"

class haar_decomposer : public wavelet_decomposer
{
public:
	haar_decomposer(ovc::wavelet_config in_config);

	virtual ovc::wavelet_decomposition_2d<double> decompose(const matrix<double>& in_x, size_t in_num_levels) override;
};