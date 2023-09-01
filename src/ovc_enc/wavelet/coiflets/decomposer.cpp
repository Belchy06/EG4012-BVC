#include "ovc_common/wavelet/coiflets/coiflets.h"
#include "ovc_enc/wavelet/coiflets/decomposer.h"

coiflets_decomposer::coiflets_decomposer(ovc::wavelet_config in_config)
{
	using namespace ovc;
	switch (in_config.coiflets_config)
	{
		case WAVELET_COIFLETS_1:
			wave = new wavelet<double>(coiflets1::lo_d, coiflets1::hi_d, coiflets1::lo_r, coiflets1::hi_r);
			break;
		case WAVELET_COIFLETS_2:
			wave = new wavelet<double>(coiflets2::lo_d, coiflets2::hi_d, coiflets2::lo_r, coiflets2::hi_r);
			break;
		case WAVELET_COIFLETS_3:
			wave = new wavelet<double>(coiflets3::lo_d, coiflets3::hi_d, coiflets3::lo_r, coiflets3::hi_r);
			break;
		case WAVELET_COIFLETS_4:
			wave = new wavelet<double>(coiflets4::lo_d, coiflets4::hi_d, coiflets4::lo_r, coiflets4::hi_r);
			break;
		case WAVELET_COIFLETS_5:
			wave = new wavelet<double>(coiflets5::lo_d, coiflets5::hi_d, coiflets5::lo_r, coiflets5::hi_r);
			break;
		default:
			wave = nullptr;
			break;
	}
}

ovc::wavelet_decomposition_2d<double> coiflets_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	return wave->decompose(in_x, in_num_levels);
}