#pragma once

#include "bvc_common/wavelet/wavelet.h"

class bvc_wavelet_decomposer
{
public:
	virtual bvc_wavelet_decomposition_2d<double> decompose(const matrix<double>& in_x, size_t in_num_levels) = 0;

protected:
	wavelet<double>* wave;
};