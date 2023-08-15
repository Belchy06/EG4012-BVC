#include <cmath>

#include "bvc_common/log.h"
#include "bvc_enc/wavelet/wavelet_decomposer.h"
#include "bvc_enc/wavelet/wavelet_decomposer_factory.h"
#include "bvc_dec/wavelet/wavelet_reconstructor.h"
#include "bvc_dec/wavelet/wavelet_reconstructor_factory.h"

#include "wavelet_test.h"

#define LogWaveletTest "LogWaveletTest"

bool wavelet_test::test(bvc_wavelet in_wavelet, bvc_wavelet_config in_config, size_t in_x, size_t in_y, size_t in_num_levels)
{
	matrix<double> raw(in_x, in_y);
	raw.rand(0);

	bvc_wavelet_decomposition_2d<double> decomposition = wavelet_test::decompose(in_wavelet, in_config, raw, in_num_levels);

	matrix<double> reconstruction = wavelet_test::reconstruct(in_wavelet, in_config, decomposition, raw.size());

	double norm = bvc_vector::norm((raw - reconstruction).get_data());

	LOG(LogWaveletTest, BVC_VERBOSITY_VERBOSE, "l2-norm: {}", norm);
	return trunc(norm) == 0;
}

bvc_wavelet_decomposition_2d<double> wavelet_test::decompose(bvc_wavelet in_wavelet, bvc_wavelet_config in_config, const matrix<double>& in_x, size_t in_num_levels)
{
	std::shared_ptr<bvc_wavelet_decomposer> wavelet_decomposer = bvc_wavelet_decomposer_factory::create_wavelet_decomposer(in_wavelet, in_config);

	return wavelet_decomposer->decompose(in_x, in_num_levels);
}

matrix<double> wavelet_test::reconstruct(bvc_wavelet in_wavelet, bvc_wavelet_config in_config, const bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec)
{
	std::shared_ptr<bvc_wavelet_reconstructor> wavelet_reconstructor = bvc_wavelet_reconstructor_factory::create_wavelet_reconstructor(in_wavelet, in_config);

	return wavelet_reconstructor->reconstruct(in_decomposition, in_size_rec);
}