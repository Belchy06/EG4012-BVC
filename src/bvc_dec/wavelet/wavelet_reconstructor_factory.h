#pragma once

#include <memory>

#include "bvc_common/wavelet/wavelet.h"
#include "bvc_common/wavelet/wavelet_config.h"
#include "wavelet_reconstructor.h"

class bvc_wavelet_reconstructor_factory
{
public:
	static std::shared_ptr<bvc_wavelet_reconstructor> create_wavelet_reconstructor(bvc_wavelet in_wavelet, bvc_wavelet_config in_config);
};