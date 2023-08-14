#include "wavelet_decomposer_factory.h"
#include "haar/haar_decomposer.h"

template <typename T>
std::shared_ptr<bvc_wavelet_decomposer<T>> bvc_wavelet_decomposer_factory<T>::create_wavelet_decomposer(bvc_wavelet in_wavelet)
{
	switch (in_wavelet)
	{
		case bvc_wavelet::BVC_WAVELET_HAAR:
			return std::make_shared<haar_decomposer<T>>();
		case bvc_wavelet::BVC_WAVELET_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}