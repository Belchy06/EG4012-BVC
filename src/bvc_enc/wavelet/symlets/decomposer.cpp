#include "bvc_common/wavelet/symlets/symlets.h"
#include "bvc_enc/wavelet/symlets/decomposer.h"

symlets_decomposer::symlets_decomposer(bvc_wavelet_config in_config)
{
	switch (in_config.symlets_config)
	{
		case BVC_WAVELET_SYMLETS_2:
			wave = new wavelet<double>(symlets2::lo_d, symlets2::hi_d, symlets2::lo_r, symlets2::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_3:
			wave = new wavelet<double>(symlets3::lo_d, symlets3::hi_d, symlets3::lo_r, symlets3::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_4:
			wave = new wavelet<double>(symlets4::lo_d, symlets4::hi_d, symlets4::lo_r, symlets4::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_5:
			wave = new wavelet<double>(symlets5::lo_d, symlets5::hi_d, symlets5::lo_r, symlets5::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_6:
			wave = new wavelet<double>(symlets6::lo_d, symlets6::hi_d, symlets6::lo_r, symlets6::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_7:
			wave = new wavelet<double>(symlets7::lo_d, symlets7::hi_d, symlets7::lo_r, symlets7::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_8:
			wave = new wavelet<double>(symlets8::lo_d, symlets8::hi_d, symlets8::lo_r, symlets8::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_9:
			wave = new wavelet<double>(symlets9::lo_d, symlets9::hi_d, symlets9::lo_r, symlets9::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_10:
			wave = new wavelet<double>(symlets10::lo_d, symlets10::hi_d, symlets10::lo_r, symlets10::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_11:
			wave = new wavelet<double>(symlets11::lo_d, symlets11::hi_d, symlets11::lo_r, symlets11::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_12:
			wave = new wavelet<double>(symlets12::lo_d, symlets12::hi_d, symlets12::lo_r, symlets12::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_13:
			wave = new wavelet<double>(symlets13::lo_d, symlets13::hi_d, symlets13::lo_r, symlets13::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_14:
			wave = new wavelet<double>(symlets14::lo_d, symlets14::hi_d, symlets14::lo_r, symlets14::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_15:
			wave = new wavelet<double>(symlets15::lo_d, symlets15::hi_d, symlets15::lo_r, symlets15::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_16:
			wave = new wavelet<double>(symlets16::lo_d, symlets16::hi_d, symlets16::lo_r, symlets16::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_17:
			wave = new wavelet<double>(symlets17::lo_d, symlets17::hi_d, symlets17::lo_r, symlets17::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_18:
			wave = new wavelet<double>(symlets18::lo_d, symlets18::hi_d, symlets18::lo_r, symlets18::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_19:
			wave = new wavelet<double>(symlets19::lo_d, symlets19::hi_d, symlets19::lo_r, symlets19::hi_r);
			break;
		case BVC_WAVELET_SYMLETS_20:
			wave = new wavelet<double>(symlets20::lo_d, symlets20::hi_d, symlets20::lo_r, symlets20::hi_r);
			break;
		default:
			wave = nullptr;
			break;
	}
}

bvc_wavelet_decomposition_2d<double> symlets_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	return wave->decompose(in_x, in_num_levels);
}