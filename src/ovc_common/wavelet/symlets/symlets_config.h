#pragma once

#include <stdint.h>
#include <string>

namespace ovc
{
	typedef enum : uint8_t
	{
		WAVELET_SYMLETS_2,
		WAVELET_SYMLETS_3,
		WAVELET_SYMLETS_4,
		WAVELET_SYMLETS_5,
		WAVELET_SYMLETS_6,
		WAVELET_SYMLETS_7,
		WAVELET_SYMLETS_8,
		WAVELET_SYMLETS_9,
		WAVELET_SYMLETS_10,
		WAVELET_SYMLETS_11,
		WAVELET_SYMLETS_12,
		WAVELET_SYMLETS_13,
		WAVELET_SYMLETS_14,
		WAVELET_SYMLETS_15,
		WAVELET_SYMLETS_16,
		WAVELET_SYMLETS_17,
		WAVELET_SYMLETS_18,
		WAVELET_SYMLETS_19,
		WAVELET_SYMLETS_20
	} wavelet_symlets_config;

	inline std::string symlets_config_to_string(wavelet_symlets_config in_symlets_config)
	{
		switch (in_symlets_config)
		{
			case WAVELET_SYMLETS_2:
				return "WAVELET_SYMLETS_2";
			case WAVELET_SYMLETS_3:
				return "WAVELET_SYMLETS_3";
			case WAVELET_SYMLETS_4:
				return "WAVELET_SYMLETS_4";
			case WAVELET_SYMLETS_5:
				return "WAVELET_SYMLETS_5";
			case WAVELET_SYMLETS_6:
				return "WAVELET_SYMLETS_6";
			case WAVELET_SYMLETS_7:
				return "WAVELET_SYMLETS_7";
			case WAVELET_SYMLETS_8:
				return "WAVELET_SYMLETS_8";
			case WAVELET_SYMLETS_9:
				return "WAVELET_SYMLETS_9";
			case WAVELET_SYMLETS_10:
				return "WAVELET_SYMLETS_10";
			case WAVELET_SYMLETS_11:
				return "WAVELET_SYMLETS_11";
			case WAVELET_SYMLETS_12:
				return "WAVELET_SYMLETS_12";
			case WAVELET_SYMLETS_13:
				return "WAVELET_SYMLETS_13";
			case WAVELET_SYMLETS_14:
				return "WAVELET_SYMLETS_14";
			case WAVELET_SYMLETS_15:
				return "WAVELET_SYMLETS_15";
			case WAVELET_SYMLETS_16:
				return "WAVELET_SYMLETS_16";
			case WAVELET_SYMLETS_17:
				return "WAVELET_SYMLETS_17";
			case WAVELET_SYMLETS_18:
				return "WAVELET_SYMLETS_18";
			case WAVELET_SYMLETS_19:
				return "WAVELET_SYMLETS_19";
			case WAVELET_SYMLETS_20:
				return "WAVELET_SYMLETS_20";
			default:
				return "";
		}
	}
} // namespace ovc