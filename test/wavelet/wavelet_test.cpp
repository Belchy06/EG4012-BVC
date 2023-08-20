#include <cmath>

#include "bvc_common/log.h"
#include "bvc_enc/wavelet/decomposer.h"
#include "bvc_enc/wavelet/decomposer_factory.h"
#include "bvc_dec/wavelet/recomposer.h"
#include "bvc_dec/wavelet/recomposer_factory.h"

#include "wavelet_test.h"

#define LogWaveletTest "LogWaveletTest"

bool wavelet_test::test(bvc_wavelet_family in_wavelet_family, bvc_wavelet_config in_config, size_t in_x, size_t in_y, size_t in_num_levels)
{
	matrix<double> raw(in_x, in_y);
	raw.rand(0);

	bvc_wavelet_decomposition_2d<double> decomposition = wavelet_test::decompose(in_wavelet_family, in_config, raw, in_num_levels);

	matrix<double> recomposition = wavelet_test::recompose(in_wavelet_family, in_config, decomposition, raw.size());

	double norm = bvc_vector::norm((raw - recomposition).get_data());

	LOG(LogWaveletTest, BVC_VERBOSITY_VERBOSE, "l2-norm: {}", norm);
	return trunc(norm) == 0;
}

bvc_wavelet_decomposition_2d<double> wavelet_test::decompose(bvc_wavelet_family in_wavelet_family, bvc_wavelet_config in_config, const matrix<double>& in_x, size_t in_num_levels)
{
	std::shared_ptr<bvc_wavelet_decomposer> wavelet_decomposer = bvc_wavelet_decomposer_factory::create_wavelet_decomposer(in_wavelet_family, in_config);

	return wavelet_decomposer->decompose(in_x, in_num_levels);
}

matrix<double> wavelet_test::recompose(bvc_wavelet_family in_wavelet_family, bvc_wavelet_config in_config, const bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec)
{
	std::shared_ptr<bvc_wavelet_recomposer> wavelet_recomposer = bvc_wavelet_recomposer_factory::create_wavelet_recomposer(in_wavelet_family, in_config);

	return wavelet_recomposer->recompose(in_decomposition, in_size_rec);
}