#include "ovc_common/wavelet/haar/haar.h"
#include "ovc_enc/wavelet/haar/decomposer.h"

haar_decomposer::haar_decomposer(ovc::wavelet_config in_config)
{
	using namespace ovc;
	switch (in_config.haar_config)
	{
		case WAVELET_HAAR_DEFAULT:
			wave = new wavelet<double>(haar::lo_d, haar::hi_d, haar::lo_r, haar::hi_r);
			break;
		default:
			wave = nullptr;
			break;
	}
}

ovc::wavelet_decomposition_2d<double> haar_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	return wave->decompose(in_x, in_num_levels);
}