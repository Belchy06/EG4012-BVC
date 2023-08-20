#include "ovc_enc/wavelet/biorthogonal/decomposer.h"
#include "ovc_enc/wavelet/coiflets/decomposer.h"
#include "ovc_enc/wavelet/daubechies/decomposer.h"
#include "ovc_enc/wavelet/haar/decomposer.h"
#include "ovc_enc/wavelet/reverse_biorthogonal/decomposer.h"
#include "ovc_enc/wavelet/skip/decomposer.h"
#include "ovc_enc/wavelet/symlets/decomposer.h"
#include "ovc_enc/wavelet/decomposer_factory.h"

std::shared_ptr<ovc_wavelet_decomposer> ovc_wavelet_decomposer_factory::create_wavelet_decomposer(ovc_wavelet_family in_wavelet_transform, ovc_wavelet_config in_config)
{
	switch (in_wavelet_transform)
	{
		case OVC_WAVELET_FAMILY_BIORTHOGONAL:
			return std::make_shared<biorthogonal_decomposer>(in_config);
		case OVC_WAVELET_FAMILY_COIFLETS:
			return std::make_shared<coiflets_decomposer>(in_config);
		case OVC_WAVELET_FAMILY_DAUBECHIES:
			return std::make_shared<daubechies_decomposer>(in_config);
		case OVC_WAVELET_FAMILY_HAAR:
			return std::make_shared<haar_decomposer>(in_config);
		case OVC_WAVELET_FAMILY_REVERSE_BIORTHOGONAL:
			return std::make_shared<reverse_biorthogonal_decomposer>(in_config);
		case OVC_WAVELET_FAMILY_SKIP:
			return std::make_shared<skip_decomposer>(in_config);
		case OVC_WAVELET_FAMILY_SYMLETS:
			return std::make_shared<symlets_decomposer>(in_config);
		default:
			return nullptr;
	}
}