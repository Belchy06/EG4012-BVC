#pragma once

#include <memory>

#include "bvc_common/wavelet/wavelet.h"
#include "wavelet_decomposer.h"

template <typename T>
class bvc_wavelet_decomposer_factory
{
public:
	static std::shared_ptr<bvc_wavelet_decomposer<T>> create_wavelet_decomposer(bvc_wavelet in_wavelet);
};