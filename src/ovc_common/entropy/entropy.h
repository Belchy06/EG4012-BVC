#pragma once

#include <string>

typedef enum
{
	OVC_ENTROPY_CODER_SKIP,
	OVC_ENTROPY_CODER_ARITHMETIC,
	OVC_ENTROPY_CODER_HUFFMAN
} ovc_entropy_coder;

inline std::string entropy_coder_to_string(ovc_entropy_coder in_entropy_coder)
{
	switch (in_entropy_coder)
	{
		case OVC_ENTROPY_CODER_SKIP:
			return "OVC_ENTROPY_CODER_SKIP";
		case OVC_ENTROPY_CODER_ARITHMETIC:
			return "OVC_ENTROPY_CODER_ARITHMETIC";
		case OVC_ENTROPY_CODER_HUFFMAN:
			return "OVC_ENTROPY_CODER_HUFFMAN";
		default:
			return "";
	}
}
