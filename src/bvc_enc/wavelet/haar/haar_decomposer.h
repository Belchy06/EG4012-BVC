#pragma once

#include "bvc_enc/wavelet/wavelet_decomposer.h"

template <typename T>
class haar_decomposer : public bvc_wavelet_decomposer<T>
{
public:
	haar_decomposer();

	virtual bvc_wavelet_decomposition_2d<T> decompose(const matrix<T>& in_x, size_t in_num_levels) override;
};