#pragma once

#include "ovc_common/wavelet/wavelet.h"

class ovc_wavelet_recomposer
{
public:
	virtual matrix<double> recompose(ovc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const = 0;

protected:
	wavelet<double>* wave;
};