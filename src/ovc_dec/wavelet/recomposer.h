#pragma once

#include "ovc_common/wavelet/wavelet.h"

class wavelet_recomposer
{
public:
	virtual matrix<double> recompose(ovc::wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const = 0;

protected:
	ovc::wavelet<double>* wave;
};