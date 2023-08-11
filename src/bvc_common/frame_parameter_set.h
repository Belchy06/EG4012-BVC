#pragma once

/**
 * The 'Frame Parameter Set' contains information needed to decode a frame.
 *
 * This information is stored at the beginning of each NAL
 */
class frame_parameter_set
{
	size_t width;
	size_t height;
};