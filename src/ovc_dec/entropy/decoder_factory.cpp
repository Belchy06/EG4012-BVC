#include "ovc_dec/entropy/decoder_factory.h"
#include "ovc_dec/entropy/arithmetic/decoder.h"
#include "ovc_dec/entropy/huffman/decoder.h"
#include "ovc_dec/entropy/skip/decoder.h"

std::shared_ptr<ovc_entropy_decoder> ovc_entropy_decoder_factory::create_entropy_decoder(ovc_entropy_coder in_coder)
{
	switch (in_coder)
	{
		case OVC_ENTROPY_CODER_SKIP:
			return std::make_shared<skip_entropy_decoder>();
		case OVC_ENTROPY_CODER_ARITHMETIC:
			return std::make_shared<arithmetic_decoder>();
		case OVC_ENTROPY_CODER_HUFFMAN:
			return std::make_shared<huffman_decoder>();
		default:
			return nullptr;
	}
}