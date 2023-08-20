#pragma once

#include <memory>

#include "ovc_common/wavelet/wavelet.h"
#include "ovc_common/wavelet/wavelet_config.h"
#include "ovc_enc/wavelet/decomposer.h"

class ovc_wavelet_decomposer_factory
{
public:
	static std::shared_ptr<ovc_wavelet_decomposer> create_wavelet_decomposer(ovc_wavelet_family in_wavelet_transform, ovc_wavelet_config in_config);
};