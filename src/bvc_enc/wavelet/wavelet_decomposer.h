#pragma once

#include "bvc_common/wavelet/matrix.h"
#include "bvc_common/wavelet/wavelet.h"

template <typename T>
class bvc_wavelet_decomposer
{
public:
	virtual bvc_wavelet_decomposition_2d<T> decompose(const matrix<T>& in_x, size_t in_num_levels) = 0;

protected:
	wavelet<double> wav;
};