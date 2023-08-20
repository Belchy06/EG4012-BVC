#pragma once

#include "bvc_common/wavelet/wavelet_config.h"
#include "bvc_dec/wavelet/recomposer.h"

class haar_recomposer : public bvc_wavelet_recomposer
{
public:
	haar_recomposer(bvc_wavelet_config in_config);

	virtual matrix<double> recompose(const bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const override;
};