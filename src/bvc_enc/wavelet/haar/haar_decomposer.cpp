#include "bvc_common/wavelet/haar/haar.h"
#include "haar_decomposer.h"

template <typename T>
haar_decomposer<T>::haar_decomposer()
	: wav(haar::lo_d, haar::hi_d, haar::lo_r, haar::hi_r)
{
}

template <typename T>
bvc_wavelet_decomposition_2d<T> haar_decomposer<T>::decompose(const matrix<T>& in_x, size_t in_num_levels)
{
	return wav.decompose(in_x, in_num_levels);
}