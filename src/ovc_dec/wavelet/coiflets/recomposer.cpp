#include "ovc_common/wavelet/coiflets/coiflets.h"
#include "ovc_dec/wavelet/coiflets/recomposer.h"

coiflets_recomposer::coiflets_recomposer(ovc_wavelet_config in_config)
{
	switch (in_config.coiflets_config)
	{
		case OVC_WAVELET_COIFLETS_1:
			wave = new wavelet<double>(coiflets1::lo_d, coiflets1::hi_d, coiflets1::lo_r, coiflets1::hi_r);
			break;
		case OVC_WAVELET_COIFLETS_2:
			wave = new wavelet<double>(coiflets2::lo_d, coiflets2::hi_d, coiflets2::lo_r, coiflets2::hi_r);
			break;
		case OVC_WAVELET_COIFLETS_3:
			wave = new wavelet<double>(coiflets3::lo_d, coiflets3::hi_d, coiflets3::lo_r, coiflets3::hi_r);
			break;
		case OVC_WAVELET_COIFLETS_4:
			wave = new wavelet<double>(coiflets4::lo_d, coiflets4::hi_d, coiflets4::lo_r, coiflets4::hi_r);
			break;
		case OVC_WAVELET_COIFLETS_5:
			wave = new wavelet<double>(coiflets5::lo_d, coiflets5::hi_d, coiflets5::lo_r, coiflets5::hi_r);
			break;
		default:
			wave = nullptr;
			break;
	}
}

matrix<double> coiflets_recomposer::recompose(ovc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const
{
	return wave->reconstruct(in_decomposition, in_size_rec);
}