#include "biorthogonal/biorthogonal_decomposer.h"
#include "coiflets/coiflets_decomposer.h"
#include "haar/haar_decomposer.h"

#include "wavelet_decomposer_factory.h"

std::shared_ptr<bvc_wavelet_decomposer> bvc_wavelet_decomposer_factory::create_wavelet_decomposer(bvc_wavelet in_wavelet, bvc_wavelet_config in_config)
{
	switch (in_wavelet)
	{
		case bvc_wavelet::BVC_WAVELET_HAAR:
			return std::make_shared<haar_decomposer>(in_config);
		case bvc_wavelet::BVC_WAVELET_BIORTHOGONAL:
			return std::make_shared<biorthogonal_decomposer>(in_config);
		case bvc_wavelet::BVC_WAVELET_COIFLETS:
			return std::make_shared<coiflets_decomposer>(in_config);
		case bvc_wavelet::BVC_WAVELET_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}