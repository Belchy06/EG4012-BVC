#pragma once

#include <memory>

#include "ovc_common/wavelet/wavelet.h"
#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_enc/wavelet/decomposer.h"

class wavelet_decomposer_factory
{
public:
	static std::shared_ptr<wavelet_decomposer> create_wavelet_decomposer(ovc::wavelet_family in_wavelet_transform, ovc::wavelet_config in_config);
};