#pragma once

#include "bvc_common/wavelet/wavelet_config.h"
#include "bvc_dec/wavelet/wavelet_reconstructor.h"

class haar_reconstructor : public bvc_wavelet_reconstructor
{
public:
	haar_reconstructor(bvc_wavelet_config in_config);

	virtual matrix<double> reconstruct(const bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const override;
};