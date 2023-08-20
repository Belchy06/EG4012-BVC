#include "ovc_enc/wavelet/reverse_biorthogonal/decomposer.h"

#include "ovc_common/wavelet/reverse_biorthogonal/reverse_biorthogonal.h"

reverse_biorthogonal_decomposer::reverse_biorthogonal_decomposer(ovc_wavelet_config in_config)
{
	switch (in_config.reverse_biorthogonal_config)
	{
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_1p1:
			wave = new wavelet<double>(reverse_biorthogonal1p1::lo_d, reverse_biorthogonal1p1::hi_d, reverse_biorthogonal1p1::lo_r, reverse_biorthogonal1p1::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_1p3:
			wave = new wavelet<double>(reverse_biorthogonal1p3::lo_d, reverse_biorthogonal1p3::hi_d, reverse_biorthogonal1p3::lo_r, reverse_biorthogonal1p3::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_1p5:
			wave = new wavelet<double>(reverse_biorthogonal1p5::lo_d, reverse_biorthogonal1p5::hi_d, reverse_biorthogonal1p5::lo_r, reverse_biorthogonal1p5::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_2p2:
			wave = new wavelet<double>(reverse_biorthogonal2p2::lo_d, reverse_biorthogonal2p2::hi_d, reverse_biorthogonal2p2::lo_r, reverse_biorthogonal2p2::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_2p4:
			wave = new wavelet<double>(reverse_biorthogonal2p4::lo_d, reverse_biorthogonal2p4::hi_d, reverse_biorthogonal2p4::lo_r, reverse_biorthogonal2p4::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_2p6:
			wave = new wavelet<double>(reverse_biorthogonal2p6::lo_d, reverse_biorthogonal2p6::hi_d, reverse_biorthogonal2p6::lo_r, reverse_biorthogonal2p6::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_2p8:
			wave = new wavelet<double>(reverse_biorthogonal2p8::lo_d, reverse_biorthogonal2p8::hi_d, reverse_biorthogonal2p8::lo_r, reverse_biorthogonal2p8::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_3p1:
			wave = new wavelet<double>(reverse_biorthogonal3p1::lo_d, reverse_biorthogonal3p1::hi_d, reverse_biorthogonal3p1::lo_r, reverse_biorthogonal3p1::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_3p3:
			wave = new wavelet<double>(reverse_biorthogonal3p3::lo_d, reverse_biorthogonal3p3::hi_d, reverse_biorthogonal3p3::lo_r, reverse_biorthogonal3p3::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_3p5:
			wave = new wavelet<double>(reverse_biorthogonal3p5::lo_d, reverse_biorthogonal3p5::hi_d, reverse_biorthogonal3p5::lo_r, reverse_biorthogonal3p5::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_3p7:
			wave = new wavelet<double>(reverse_biorthogonal3p7::lo_d, reverse_biorthogonal3p7::hi_d, reverse_biorthogonal3p7::lo_r, reverse_biorthogonal3p7::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_3p9:
			wave = new wavelet<double>(reverse_biorthogonal3p9::lo_d, reverse_biorthogonal3p9::hi_d, reverse_biorthogonal3p9::lo_r, reverse_biorthogonal3p9::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_4p4:
			wave = new wavelet<double>(reverse_biorthogonal4p4::lo_d, reverse_biorthogonal4p4::hi_d, reverse_biorthogonal4p4::lo_r, reverse_biorthogonal4p4::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_5p5:
			wave = new wavelet<double>(reverse_biorthogonal5p5::lo_d, reverse_biorthogonal5p5::hi_d, reverse_biorthogonal5p5::lo_r, reverse_biorthogonal5p5::hi_r);
			break;
		case OVC_WAVELET_REVERSE_BIORTHOGONAL_6p8:
			wave = new wavelet<double>(reverse_biorthogonal6p8::lo_d, reverse_biorthogonal6p8::hi_d, reverse_biorthogonal6p8::lo_r, reverse_biorthogonal6p8::hi_r);
			break;
		default:
			wave = nullptr;
			break;
	}
}

ovc_wavelet_decomposition_2d<double> reverse_biorthogonal_decomposer::decompose(const matrix<double>& in_x, size_t in_num_levels)
{
	return wave->decompose(in_x, in_num_levels);
}