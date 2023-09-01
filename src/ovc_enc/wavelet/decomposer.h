#pragma once

#include "ovc_common/wavelet/wavelet.h"

class wavelet_decomposer
{
public:
	virtual ovc::wavelet_decomposition_2d<double> decompose(const matrix<double>& in_x, size_t in_num_levels) = 0;

protected:
	ovc::wavelet<double>* wave;
};