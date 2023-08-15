#include "bvc_common/wavelet/haar/haar.h"
#include "haar_reconstructor.h"

haar_reconstructor::haar_reconstructor()
{
	wave = new wavelet<double>(haar::lo_d, haar::hi_d, haar::lo_r, haar::hi_r);
}

matrix<double> haar_reconstructor::reconstruct(const bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const
{
	return wave->reconstruct(in_decomposition, in_size_rec);
}