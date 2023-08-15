#pragma once

#include "bvc_enc/wavelet/wavelet_decomposer.h"

class haar_decomposer : public bvc_wavelet_decomposer
{
public:
	haar_decomposer();

	virtual bvc_wavelet_decomposition_2d<double> decompose(const matrix<double>& in_x, size_t in_num_levels) override;
};