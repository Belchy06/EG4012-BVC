#pragma once

#include <memory>

#include "bvc_common/wavelet/wavelet.h"
#include "bvc_common/wavelet/wavelet_config.h"

#include "bvc_enc/wavelet/wavelet_decomposer.h"

class bvc_wavelet_decomposer_factory
{
public:
	static std::shared_ptr<bvc_wavelet_decomposer> create_wavelet_decomposer(bvc_wavelet in_wavelet, bvc_wavelet_config in_config);
};