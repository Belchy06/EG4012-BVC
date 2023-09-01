#include "ovc_dec/compress/decompressor_factory.h"
#include "ovc_dec/compress/skip/decompressor.h"
#include "ovc_dec/compress/spiht/decompressor.h"

std::shared_ptr<decompressor> decompressor_factory::create_decompressor(ovc::compression in_compression)
{
	switch (in_compression)
	{
		case ovc::COMPRESSION_SKIP:
			return std::make_shared<skip_decompressor>();
		case ovc::COMPRESSION_SPIHT:
			return std::make_shared<spiht_decompressor>();
		default:
			return nullptr;
	}
}