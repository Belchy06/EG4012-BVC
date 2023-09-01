#pragma once

#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_dec/wavelet/recomposer.h"

class reverse_biorthogonal_recomposer : public wavelet_recomposer
{
public:
	reverse_biorthogonal_recomposer(ovc::wavelet_config in_config);

	virtual matrix<double> recompose(ovc::wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const override;
};