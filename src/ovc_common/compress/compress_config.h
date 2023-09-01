#pragma once

namespace ovc
{
	typedef struct compression_config
	{
		float  bpp;
		size_t num_levels;
		int	   step;
	} compression_config;
} // namespace ovc
