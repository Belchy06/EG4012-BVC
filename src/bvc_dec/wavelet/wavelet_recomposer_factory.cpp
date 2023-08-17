#include "bvc_dec/wavelet/biorthogonal/recomposer.h"
#include "bvc_dec/wavelet/coiflets/recomposer.h"
#include "bvc_dec/wavelet/haar/recomposer.h"
#include "bvc_dec/wavelet/wavelet_recomposer_factory.h"

std::shared_ptr<bvc_wavelet_recomposer> bvc_wavelet_recomposer_factory::create_wavelet_recomposer(bvc_wavelet in_wavelet, bvc_wavelet_config in_config)
{
	switch (in_wavelet)
	{
		case bvc_wavelet::BVC_WAVELET_HAAR:
			return std::make_shared<haar_recomposer>(in_config);
		case bvc_wavelet::BVC_WAVELET_BIORTHOGONAL:
			return std::make_shared<biorthogonal_recomposer>(in_config);
		case bvc_wavelet::BVC_WAVELET_COIFLETS:
			return std::make_shared<coiflets_recomposer>(in_config);
		default:
			return nullptr;
	}
}