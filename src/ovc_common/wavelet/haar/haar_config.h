#pragma once

#include <stdint.h>
#include <string>

typedef enum : uint8_t
{
	OVC_WAVELET_HAAR_DEFAULT
} ovc_wavelet_haar_config;

inline std::string haar_config_to_string(ovc_wavelet_haar_config in_haar_config)
{
	switch (in_haar_config)
	{
		case OVC_WAVELET_HAAR_DEFAULT:
			return "OVC_WAVELET_HAAR_DEFAULT";
		default:
			return "";
	}
}