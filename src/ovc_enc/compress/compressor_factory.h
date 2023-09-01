#pragma once

#include <memory>

#include "ovc_common/compress/compress.h"
#include "ovc_enc/compress/compressor.h"

class compressor_factory
{
public:
	static std::shared_ptr<compressor> create_compressor(ovc::compression in_compression);
};