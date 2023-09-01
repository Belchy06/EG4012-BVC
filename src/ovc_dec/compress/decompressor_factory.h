#pragma once

#include <memory>

#include "ovc_common/compress/compress.h"
#include "ovc_dec/compress/decompressor.h"

class decompressor_factory
{
public:
	static std::shared_ptr<decompressor> create_decompressor(ovc::compression in_compression);
};
