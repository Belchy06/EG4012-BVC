#include "bvc_common/wavelet/biorthogonal/biorthogonal.h"
#include "biorthogonal_reconstructor.h"

biorthogonal_reconstructor::biorthogonal_reconstructor()
{
	wave = new wavelet<double>(biorthogonal::lo_d, biorthogonal::hi_d, biorthogonal::lo_r, biorthogonal::hi_r);
}

matrix<double> biorthogonal_reconstructor::reconstruct(const bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const
{
	return wave->reconstruct(in_decomposition, in_size_rec);
}