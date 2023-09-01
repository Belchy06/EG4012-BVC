#pragma once

#include <string>

namespace ovc
{
	typedef enum
	{
		COMPRESSION_SKIP,
		COMPRESSION_SPIHT,
	} compression;

	inline std::string compression_to_string(compression in_compression)
	{
		switch (in_compression)
		{
			case COMPRESSION_SKIP:
				return "COMPRESSION_SKIP";
			case COMPRESSION_SPIHT:
				return "COMPRESSION_SPIHT";
			default:
				return "";
		}
	}
}