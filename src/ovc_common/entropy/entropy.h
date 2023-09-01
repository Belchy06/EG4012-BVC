#pragma once

#include <string>

namespace ovc
{
	typedef enum
	{
		ENTROPY_CODER_SKIP,
		ENTROPY_CODER_ARITHMETIC,
		ENTROPY_CODER_HUFFMAN
	} entropy_coder;

	inline std::string entropy_coder_to_string(entropy_coder in_entropy_coder)
	{
		switch (in_entropy_coder)
		{
			case ENTROPY_CODER_SKIP:
				return "ENTROPY_CODER_SKIP";
			case ENTROPY_CODER_ARITHMETIC:
				return "ENTROPY_CODER_ARITHMETIC";
			case ENTROPY_CODER_HUFFMAN:
				return "ENTROPY_CODER_HUFFMAN";
			default:
				return "";
		}
	}
} // namespace ovc
