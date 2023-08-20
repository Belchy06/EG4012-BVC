#pragma once

#include <stdint.h>

#include "ovc_common/wavelet/wavelet.h"
#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_common/verbosity.h"

class wavelet_test
{
public:
	static bool test(ovc_wavelet_family in_wavelet_family, ovc_wavelet_config in_config, size_t in_x, size_t in_y, size_t in_num_levels);

private:
	static ovc_wavelet_decomposition_2d<double> decompose(ovc_wavelet_family in_wavelet_family, ovc_wavelet_config in_config, const matrix<double>& in_x, size_t in_num_levels);
	static matrix<double>						recompose(ovc_wavelet_family in_wavelet_family, ovc_wavelet_config in_config, ovc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec);
};