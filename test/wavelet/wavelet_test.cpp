#include <cmath>

#include "ovc_common/log.h"
#include "ovc_enc/wavelet/decomposer.h"
#include "ovc_enc/wavelet/decomposer_factory.h"
#include "ovc_dec/wavelet/recomposer.h"
#include "ovc_dec/wavelet/recomposer_factory.h"

#include "wavelet_test.h"

#define LogWaveletTest "LogWaveletTest"

bool wavelet_test::test(ovc_wavelet_family in_wavelet_family, ovc_wavelet_config in_config, size_t in_x, size_t in_y, size_t in_num_levels)
{
	matrix<double> raw(in_x, in_y);
	raw.rand(0);

	ovc_wavelet_decomposition_2d<double> decomposition = wavelet_test::decompose(in_wavelet_family, in_config, raw, in_num_levels);

	matrix<double> recomposition = wavelet_test::recompose(in_wavelet_family, in_config, decomposition, raw.size());

	double norm = ovc_vector::norm((raw - recomposition).get_data());

	LOG(LogWaveletTest, OVC_VERBOSITY_VERBOSE, "l2-norm: {}", norm);
	return trunc(norm) == 0;
}

ovc_wavelet_decomposition_2d<double> wavelet_test::decompose(ovc_wavelet_family in_wavelet_family, ovc_wavelet_config in_config, const matrix<double>& in_x, size_t in_num_levels)
{
	std::shared_ptr<ovc_wavelet_decomposer> wavelet_decomposer = ovc_wavelet_decomposer_factory::create_wavelet_decomposer(in_wavelet_family, in_config);

	return wavelet_decomposer->decompose(in_x, in_num_levels);
}

matrix<double> wavelet_test::recompose(ovc_wavelet_family in_wavelet_family, ovc_wavelet_config in_config, ovc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec)
{
	std::shared_ptr<ovc_wavelet_recomposer> wavelet_recomposer = ovc_wavelet_recomposer_factory::create_wavelet_recomposer(in_wavelet_family, in_config);

	return wavelet_recomposer->recompose(in_decomposition, in_size_rec);
}