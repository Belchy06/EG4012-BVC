#pragma once

#include "bvc_common/wavelet/wavelet_config.h"
#include "bvc_enc/wavelet/decomposer.h"

class skip_decomposer : public bvc_wavelet_decomposer
{
public:
	skip_decomposer(bvc_wavelet_config in_config);

	virtual bvc_wavelet_decomposition_2d<double> decompose(const matrix<double>& in_x, size_t in_num_levels) override;
};