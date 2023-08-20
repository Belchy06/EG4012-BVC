#include "ovc_enc/wavelet/skip/decomposer.h"

skip_decomposer::skip_decomposer(ovc_wavelet_config in_config)
{
}

ovc_wavelet_decomposition_2d<double> skip_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	ovc_wavelet_decomposition_2d<double> faux_decomposition = ovc_wavelet_decomposition_2d<double>::from_matrix(in_x, in_num_levels);
	return faux_decomposition;
}