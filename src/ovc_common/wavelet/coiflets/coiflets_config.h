#pragma once

#include <stdint.h>
#include <string>

typedef enum : uint8_t
{
	OVC_WAVELET_COIFLETS_1,
	OVC_WAVELET_COIFLETS_2,
	OVC_WAVELET_COIFLETS_3,
	OVC_WAVELET_COIFLETS_4,
	OVC_WAVELET_COIFLETS_5
} ovc_wavelet_coiflets_config;

inline std::string coiflets_config_to_string(ovc_wavelet_coiflets_config in_coiflets_config)
{
	switch (in_coiflets_config)
	{
		case OVC_WAVELET_COIFLETS_1:
			return "OVC_WAVELET_COIFLETS_1";
		case OVC_WAVELET_COIFLETS_2:
			return "OVC_WAVELET_COIFLETS_2";
		case OVC_WAVELET_COIFLETS_3:
			return "OVC_WAVELET_COIFLETS_3";
		case OVC_WAVELET_COIFLETS_4:
			return "OVC_WAVELET_COIFLETS_4";
		case OVC_WAVELET_COIFLETS_5:
			return "OVC_WAVELET_COIFLETS_5";
		default:
			return "";
	}
}
