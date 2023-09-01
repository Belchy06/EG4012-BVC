#include "ovc_common/wavelet/haar/haar.h"
#include "ovc_dec/wavelet/haar/recomposer.h"

haar_recomposer::haar_recomposer(ovc::wavelet_config in_config)
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

matrix<double> haar_recomposer::recompose(ovc::wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const
{
	return wave->reconstruct(in_decomposition, in_size_rec);
}