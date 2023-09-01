#pragma once

#include <stdint.h>
#include <string>

namespace ovc
{
	typedef enum : uint8_t
	{
		WAVELET_HAAR_DEFAULT
	} wavelet_haar_config;

	inline std::string haar_config_to_string(wavelet_haar_config in_haar_config)
	{
		switch (in_haar_config)
		{
			case WAVELET_HAAR_DEFAULT:
				return "WAVELET_HAAR_DEFAULT";
			default:
				return "";
		}
	}
} // namespace ovc