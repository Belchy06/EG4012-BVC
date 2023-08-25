#pragma once

#include <string>

typedef enum
{
	OVC_ENC_PRESET_RESILLIENCE,
	OVC_ENC_PRESET_COMPRESSION,
	OVC_ENC_PRESET_BALANCED
} ovc_enc_preset;

inline std::string preset_to_string(ovc_enc_preset in_preset)
{
	switch (in_preset)
	{
		case OVC_ENC_PRESET_RESILLIENCE:
			return "OVC_ENC_PRESET_RESILLIENCE";
		case OVC_ENC_PRESET_COMPRESSION:
			return "OVC_ENC_PRESET_COMPRESSION";
		case OVC_ENC_PRESET_BALANCED:
			return "OVC_ENC_PRESET_BALANCED";
		default:
			return "";
	}
}