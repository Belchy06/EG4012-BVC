#pragma once

#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_enc/wavelet/decomposer.h"

class biorthogonal_decomposer : public ovc_wavelet_decomposer
{
public:
	biorthogonal_decomposer(ovc_wavelet_config in_config);

	virtual ovc_wavelet_decomposition_2d<double> decompose(const matrix<double>& in_x, size_t in_num_levels) override;
};