#pragma once

#include <memory>

#include "bvc_common/wavelet/wavelet.h"
#include "bvc_common/wavelet/wavelet_config.h"
#include "wavelet_recomposer.h"

class bvc_wavelet_recomposer_factory
{
public:
	static std::shared_ptr<bvc_wavelet_recomposer> create_wavelet_recomposer(bvc_wavelet in_wavelet, bvc_wavelet_config in_config);
};