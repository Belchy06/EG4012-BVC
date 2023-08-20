#include "bvc_dec/entropy/decoder_factory.h"
#include "bvc_dec/entropy/arithmetic/decoder.h"
#include "bvc_dec/entropy/huffman/decoder.h"
#include "bvc_dec/entropy/skip/decoder.h"

std::shared_ptr<bvc_entropy_decoder> bvc_entropy_decoder_factory::create_entropy_decoder(bvc_entropy_coder in_coder)
{
	switch (in_coder)
	{
		case BVC_ENTROPY_CODER_SKIP:
			return std::make_shared<skip_entropy_decoder>();
		case BVC_ENTROPY_CODER_ARITHMETIC:
			return std::make_shared<arithmetic_decoder>();
		case BVC_ENTROPY_CODER_HUFFMAN:
			return std::make_shared<huffman_decoder>();
		default:
			return nullptr;
	}
}