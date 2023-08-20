#include "bvc_enc/wavelet/skip/decomposer.h"

skip_decomposer::skip_decomposer(bvc_wavelet_config in_config)
{
}

bvc_wavelet_decomposition_2d<double> skip_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	bvc_wavelet_decomposition_2d<double> faux_decomposition = bvc_wavelet_decomposition_2d<double>::from_matrix(in_x, in_num_levels);
	return faux_decomposition;
}