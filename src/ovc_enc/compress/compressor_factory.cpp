#include "ovc_enc/compress/compressor_factory.h"
#include "ovc_enc/compress/skip/compressor.h"
#include "ovc_enc/compress/spiht/compressor.h"

std::shared_ptr<compressor> compressor_factory::create_compressor(ovc::compression in_compression)
{
	switch (in_compression)
	{
		case ovc::COMPRESSION_SKIP:
			return std::make_shared<skip_compressor>();
		case ovc::COMPRESSION_SPIHT:
			return std::make_shared<spiht_compressor>();
		default:
			return nullptr;
	}
}