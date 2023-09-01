#pragma once

#include <string>

namespace ovc
{
	typedef enum
	{
		PARTITION_SKIP,
		PARTITION_OFFSET_ZEROTREE,
		PARTITION_ZEROTREE_PRESERVING
	} partition;

	inline std::string partition_to_string(partition in_partition)
	{
		switch (in_partition)
		{
			case PARTITION_SKIP:
				return "PARTITION_SKIP";
			case PARTITION_OFFSET_ZEROTREE:
				return "PARTITION_OFFSET_ZEROTREE";
			case PARTITION_ZEROTREE_PRESERVING:
				return "PARTITION_ZEROTREE_PRESERVING";
			default:
				return "";
		}
	}
} // namespace ovc
