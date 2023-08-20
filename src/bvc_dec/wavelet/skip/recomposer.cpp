#include "bvc_dec/wavelet/skip/recomposer.h"

skip_recomposer::skip_recomposer(bvc_wavelet_config in_config)
{
}

matrix<double> skip_recomposer::recompose(bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const
{
	matrix<double> mat = in_decomposition.to_matrix();
	return mat;
}