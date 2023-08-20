#include "ovc_dec/wavelet/biorthogonal/recomposer.h"
#include "ovc_dec/wavelet/coiflets/recomposer.h"
#include "ovc_dec/wavelet/daubechies/recomposer.h"
#include "ovc_dec/wavelet/haar/recomposer.h"
#include "ovc_dec/wavelet/reverse_biorthogonal/recomposer.h"
#include "ovc_dec/wavelet/skip/recomposer.h"
#include "ovc_dec/wavelet/symlets/recomposer.h"
#include "ovc_dec/wavelet/recomposer_factory.h"

std::shared_ptr<ovc_wavelet_recomposer> ovc_wavelet_recomposer_factory::create_wavelet_recomposer(ovc_wavelet_family in_wavelet_transform, ovc_wavelet_config in_config)
{
	switch (in_wavelet_transform)
	{
		case OVC_WAVELET_FAMILY_BIORTHOGONAL:
			return std::make_shared<biorthogonal_recomposer>(in_config);
		case OVC_WAVELET_FAMILY_COIFLETS:
			return std::make_shared<coiflets_recomposer>(in_config);
		case OVC_WAVELET_FAMILY_DAUBECHIES:
			return std::make_shared<daubechies_recomposer>(in_config);
		case OVC_WAVELET_FAMILY_HAAR:
			return std::make_shared<haar_recomposer>(in_config);
		case OVC_WAVELET_FAMILY_REVERSE_BIORTHOGONAL:
			return std::make_shared<reverse_biorthogonal_recomposer>(in_config);
		case OVC_WAVELET_FAMILY_SKIP:
			return std::make_shared<skip_recomposer>(in_config);
		case OVC_WAVELET_FAMILY_SYMLETS:
			return std::make_shared<symlets_recomposer>(in_config);
		default:
			return nullptr;
	}
}