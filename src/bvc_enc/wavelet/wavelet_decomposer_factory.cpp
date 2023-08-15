#include "wavelet_decomposer_factory.h"
#include "haar/haar_decomposer.h"
#include "biorthogonal/biorthogonal_decomposer.h"

std::shared_ptr<bvc_wavelet_decomposer> bvc_wavelet_decomposer_factory::create_wavelet_decomposer(bvc_wavelet in_wavelet)
{
	switch (in_wavelet)
	{
		case bvc_wavelet::BVC_WAVELET_HAAR:
			return std::make_shared<haar_decomposer>();
		case bvc_wavelet::BVC_WAVELET_BIORTHOGONAL:
			return std::make_shared<biorthogonal_decomposer>();
		case bvc_wavelet::BVC_WAVELET_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}