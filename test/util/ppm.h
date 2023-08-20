#pragma once

#include "ovc_common/picture.h"

class ppm
{
public:
	static int read(std::string in_source_path, ovc_picture* out_picture);
	static int write(std::string in_output_path, ovc_picture* in_picture, std::string in_magic);
};