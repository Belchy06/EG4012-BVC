#pragma once

#include <stdint.h>
#include <string>
namespace ovc
{
	typedef enum : uint8_t
	{
		WAVELET_COIFLETS_1,
		WAVELET_COIFLETS_2,
		WAVELET_COIFLETS_3,
		WAVELET_COIFLETS_4,
		WAVELET_COIFLETS_5
	} wavelet_coiflets_config;

	inline std::string coiflets_config_to_string(wavelet_coiflets_config in_coiflets_config)
	{
		switch (in_coiflets_config)
		{
			case WAVELET_COIFLETS_1:
				return "WAVELET_COIFLETS_1";
			case WAVELET_COIFLETS_2:
				return "WAVELET_COIFLETS_2";
			case WAVELET_COIFLETS_3:
				return "WAVELET_COIFLETS_3";
			case WAVELET_COIFLETS_4:
				return "WAVELET_COIFLETS_4";
			case WAVELET_COIFLETS_5:
				return "WAVELET_COIFLETS_5";
			default:
				return "";
		}
	}
} // namespace ovc