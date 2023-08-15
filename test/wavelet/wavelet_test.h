#pragma once

#include <stdint.h>

#include "bvc_common/wavelet/wavelet.h"
#include "bvc_common/verbosity.h"

class wavelet_test
{
public:
	static bool test(bvc_wavelet in_wavelet, size_t in_x, size_t in_y, size_t in_num_levels, bvc_verbosity in_verbosity);

private:
	static bvc_wavelet_decomposition_2d<double> decompose(bvc_wavelet in_wavelet, const matrix<double>& in_x, size_t in_num_levels);
	static matrix<double>						reconstruct(bvc_wavelet in_wavelet, const bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec);
};