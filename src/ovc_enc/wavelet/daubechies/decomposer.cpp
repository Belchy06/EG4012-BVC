#include "ovc_common/wavelet/daubechies/daubechies.h"
#include "ovc_enc/wavelet/daubechies/decomposer.h"

daubechies_decomposer::daubechies_decomposer(ovc::wavelet_config in_config)
{
	using namespace ovc;
	switch (in_config.daubechies_config)
	{
		case WAVELET_DAUBECHIES_1:
			wave = new wavelet<double>(daubechies1::lo_d, daubechies1::hi_d, daubechies1::lo_r, daubechies1::hi_r);
			break;
		case WAVELET_DAUBECHIES_2:
			wave = new wavelet<double>(daubechies2::lo_d, daubechies2::hi_d, daubechies2::lo_r, daubechies2::hi_r);
			break;
		case WAVELET_DAUBECHIES_3:
			wave = new wavelet<double>(daubechies3::lo_d, daubechies3::hi_d, daubechies3::lo_r, daubechies3::hi_r);
			break;
		case WAVELET_DAUBECHIES_4:
			wave = new wavelet<double>(daubechies4::lo_d, daubechies4::hi_d, daubechies4::lo_r, daubechies4::hi_r);
			break;
		case WAVELET_DAUBECHIES_5:
			wave = new wavelet<double>(daubechies5::lo_d, daubechies5::hi_d, daubechies5::lo_r, daubechies5::hi_r);
			break;
		case WAVELET_DAUBECHIES_6:
			wave = new wavelet<double>(daubechies6::lo_d, daubechies6::hi_d, daubechies6::lo_r, daubechies6::hi_r);
			break;
		case WAVELET_DAUBECHIES_7:
			wave = new wavelet<double>(daubechies7::lo_d, daubechies7::hi_d, daubechies7::lo_r, daubechies7::hi_r);
			break;
		case WAVELET_DAUBECHIES_8:
			wave = new wavelet<double>(daubechies8::lo_d, daubechies8::hi_d, daubechies8::lo_r, daubechies8::hi_r);
			break;
		case WAVELET_DAUBECHIES_9:
			wave = new wavelet<double>(daubechies9::lo_d, daubechies9::hi_d, daubechies9::lo_r, daubechies9::hi_r);
			break;
		case WAVELET_DAUBECHIES_10:
			wave = new wavelet<double>(daubechies10::lo_d, daubechies10::hi_d, daubechies10::lo_r, daubechies10::hi_r);
			break;
		case WAVELET_DAUBECHIES_11:
			wave = new wavelet<double>(daubechies11::lo_d, daubechies11::hi_d, daubechies11::lo_r, daubechies11::hi_r);
			break;
		case WAVELET_DAUBECHIES_12:
			wave = new wavelet<double>(daubechies12::lo_d, daubechies12::hi_d, daubechies12::lo_r, daubechies12::hi_r);
			break;
		case WAVELET_DAUBECHIES_13:
			wave = new wavelet<double>(daubechies13::lo_d, daubechies13::hi_d, daubechies13::lo_r, daubechies13::hi_r);
			break;
		case WAVELET_DAUBECHIES_14:
			wave = new wavelet<double>(daubechies14::lo_d, daubechies14::hi_d, daubechies14::lo_r, daubechies14::hi_r);
			break;
		case WAVELET_DAUBECHIES_15:
			wave = new wavelet<double>(daubechies15::lo_d, daubechies15::hi_d, daubechies15::lo_r, daubechies15::hi_r);
			break;
		case WAVELET_DAUBECHIES_16:
			wave = new wavelet<double>(daubechies16::lo_d, daubechies16::hi_d, daubechies16::lo_r, daubechies16::hi_r);
			break;
		case WAVELET_DAUBECHIES_17:
			wave = new wavelet<double>(daubechies17::lo_d, daubechies17::hi_d, daubechies17::lo_r, daubechies17::hi_r);
			break;
		case WAVELET_DAUBECHIES_18:
			wave = new wavelet<double>(daubechies18::lo_d, daubechies18::hi_d, daubechies18::lo_r, daubechies18::hi_r);
			break;
		case WAVELET_DAUBECHIES_19:
			wave = new wavelet<double>(daubechies19::lo_d, daubechies19::hi_d, daubechies19::lo_r, daubechies19::hi_r);
			break;
		case WAVELET_DAUBECHIES_20:
			wave = new wavelet<double>(daubechies20::lo_d, daubechies20::hi_d, daubechies20::lo_r, daubechies20::hi_r);
			break;
		default:
			wave = nullptr;
			break;
	}
}

ovc::wavelet_decomposition_2d<double> daubechies_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	return wave->decompose(in_x, in_num_levels);
}