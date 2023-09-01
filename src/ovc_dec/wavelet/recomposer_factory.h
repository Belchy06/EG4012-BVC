#pragma once

#include <memory>

#include "ovc_common/wavelet/wavelet.h"
#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_dec/wavelet/recomposer.h"

class wavelet_recomposer_factory
{
public:
	static std::shared_ptr<wavelet_recomposer> create_wavelet_recomposer(ovc::wavelet_family in_wavelet_transform, ovc::wavelet_config in_config);
};