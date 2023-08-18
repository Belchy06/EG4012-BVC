#pragma once

#include "bvc_common/picture.h"

class ppm
{
public:
	static int read(std::string in_source_path, bvc_picture* out_picture);
	static int write(std::string in_output_path, bvc_picture* in_picture, std::string in_magic);
};