#include "ovc_dec/entropy/decoder_factory.h"
#include "ovc_dec/entropy/arithmetic/decoder.h"
#include "ovc_dec/entropy/huffman/decoder.h"
#include "ovc_dec/entropy/skip/decoder.h"

std::shared_ptr<entropy_decoder> entropy_decoder_factory::create_entropy_decoder(ovc::entropy_coder in_coder)
{
	switch (in_coder)
	{
		case ovc::ENTROPY_CODER_SKIP:
			return std::make_shared<skip_entropy_decoder>();
		case ovc::ENTROPY_CODER_ARITHMETIC:
			return std::make_shared<arithmetic_decoder>();
		case ovc::ENTROPY_CODER_HUFFMAN:
			return std::make_shared<huffman_decoder>();
		default:
			return nullptr;
	}
}