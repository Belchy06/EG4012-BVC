#pragma once

#include <string>

typedef enum
{
	OVC_PARTITION_SKIP,
	OVC_PARTITION_OFFSET_ZEROTREE,
	OVC_PARTITION_ZEROTREE_PRESERVING
} ovc_partition;

inline std::string partition_to_string(ovc_partition in_partition)
{
	switch (in_partition)
	{
		case OVC_PARTITION_SKIP:
			return "OVC_PARTITION_SKIP";
		case OVC_PARTITION_OFFSET_ZEROTREE:
			return "OVC_PARTITION_OFFSET_ZEROTREE";
		case OVC_PARTITION_ZEROTREE_PRESERVING:
			return "OVC_PARTITION_ZEROTREE_PRESERVING";
		default:
			return "";
	}
}