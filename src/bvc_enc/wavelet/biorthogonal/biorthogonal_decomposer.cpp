#include "bvc_common/wavelet/biorthogonal/biorthogonal.h"
#include "biorthogonal_decomposer.h"

biorthogonal_decomposer::biorthogonal_decomposer()
{
	wave = new wavelet<double>(biorthogonal::lo_d, biorthogonal::hi_d, biorthogonal::lo_r, biorthogonal::hi_r);
}

bvc_wavelet_decomposition_2d<double> biorthogonal_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	return wave->decompose(in_x, in_num_levels);
}