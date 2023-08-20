#include "bvc_common/wavelet/biorthogonal/biorthogonal.h"
#include "bvc_dec/wavelet/biorthogonal/recomposer.h"

biorthogonal_recomposer::biorthogonal_recomposer(bvc_wavelet_config in_config)
{
	switch (in_config.biorthogonal_config)
	{
		case BVC_WAVELET_BIORTHOGONAL_1p1:
			wave = new wavelet<double>(biorthogonal1p1::lo_d, biorthogonal1p1::hi_d, biorthogonal1p1::lo_r, biorthogonal1p1::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_1p3:
			wave = new wavelet<double>(biorthogonal1p3::lo_d, biorthogonal1p3::hi_d, biorthogonal1p3::lo_r, biorthogonal1p3::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_1p5:
			wave = new wavelet<double>(biorthogonal1p5::lo_d, biorthogonal1p5::hi_d, biorthogonal1p5::lo_r, biorthogonal1p5::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_2p2:
			wave = new wavelet<double>(biorthogonal2p2::lo_d, biorthogonal2p2::hi_d, biorthogonal2p2::lo_r, biorthogonal2p2::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_2p4:
			wave = new wavelet<double>(biorthogonal2p4::lo_d, biorthogonal2p4::hi_d, biorthogonal2p4::lo_r, biorthogonal2p4::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_2p6:
			wave = new wavelet<double>(biorthogonal2p6::lo_d, biorthogonal2p6::hi_d, biorthogonal2p6::lo_r, biorthogonal2p6::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_2p8:
			wave = new wavelet<double>(biorthogonal2p8::lo_d, biorthogonal2p8::hi_d, biorthogonal2p8::lo_r, biorthogonal2p8::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_3p1:
			wave = new wavelet<double>(biorthogonal3p1::lo_d, biorthogonal3p1::hi_d, biorthogonal3p1::lo_r, biorthogonal3p1::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_3p3:
			wave = new wavelet<double>(biorthogonal3p3::lo_d, biorthogonal3p3::hi_d, biorthogonal3p3::lo_r, biorthogonal3p3::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_3p5:
			wave = new wavelet<double>(biorthogonal3p5::lo_d, biorthogonal3p5::hi_d, biorthogonal3p5::lo_r, biorthogonal3p5::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_3p7:
			wave = new wavelet<double>(biorthogonal3p7::lo_d, biorthogonal3p7::hi_d, biorthogonal3p7::lo_r, biorthogonal3p7::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_3p9:
			wave = new wavelet<double>(biorthogonal3p9::lo_d, biorthogonal3p9::hi_d, biorthogonal3p9::lo_r, biorthogonal3p9::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_4p4:
			wave = new wavelet<double>(biorthogonal4p4::lo_d, biorthogonal4p4::hi_d, biorthogonal4p4::lo_r, biorthogonal4p4::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_5p5:
			wave = new wavelet<double>(biorthogonal5p5::lo_d, biorthogonal5p5::hi_d, biorthogonal5p5::lo_r, biorthogonal5p5::hi_r);
			break;
		case BVC_WAVELET_BIORTHOGONAL_6p8:
			wave = new wavelet<double>(biorthogonal6p8::lo_d, biorthogonal6p8::hi_d, biorthogonal6p8::lo_r, biorthogonal6p8::hi_r);
			break;
		default:
			wave = nullptr;
			break;
	}
}

matrix<double> biorthogonal_recomposer::recompose(bvc_wavelet_decomposition_2d<double>& in_decomposition, const matrix_size& in_size_rec) const
{
	return wave->reconstruct(in_decomposition, in_size_rec);
}