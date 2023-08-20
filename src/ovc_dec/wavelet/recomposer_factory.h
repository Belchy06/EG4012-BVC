#pragma once

#include <memory>

#include "ovc_common/wavelet/wavelet.h"
#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_dec/wavelet/recomposer.h"

class ovc_wavelet_recomposer_factory
{
public:
	static std::shared_ptr<ovc_wavelet_recomposer> create_wavelet_recomposer(ovc_wavelet_family in_wavelet_transform, ovc_wavelet_config in_config);
};