#include "bvc_common/wavelet/haar/haar.h"
#include "haar_decomposer.h"

haar_decomposer::haar_decomposer()
{
	wave = new wavelet<double>(haar::lo_d, haar::hi_d, haar::lo_r, haar::hi_r);
}

bvc_wavelet_decomposition_2d<double> haar_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	return wave->decompose(in_x, in_num_levels);
}