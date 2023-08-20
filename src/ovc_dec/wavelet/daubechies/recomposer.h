#pragma once

#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_dec/wavelet/recomposer.h"

class daubechies_recomposer : public ovc_wavelet_recomposer
{
public:
	daubechies_recomposer(ovc_wavelet_config in_config);

	virtual matrix<double> recompose(ovc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const override;
};