#include "wavelet_reconstructor_factory.h"
#include "haar/haar_reconstructor.h"
#include "biorthogonal/biorthogonal_reconstructor.h"

std::shared_ptr<bvc_wavelet_reconstructor> bvc_wavelet_reconstructor_factory::create_wavelet_reconstructor(bvc_wavelet in_wavelet)
{
	switch (in_wavelet)
	{
		case bvc_wavelet::BVC_WAVELET_HAAR:
			return std::make_shared<haar_reconstructor>();
		case bvc_wavelet::BVC_WAVELET_BIORTHOGONAL:
			return std::make_shared<biorthogonal_reconstructor>();
		case bvc_wavelet::BVC_WAVELET_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}