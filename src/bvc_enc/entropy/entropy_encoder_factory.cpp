#include "bvc_enc/entropy/arithmetic/encoder.h"
#include "bvc_enc/entropy/huffman/encoder.h"

#include "bvc_enc/entropy/entropy_encoder_factory.h"

std::shared_ptr<bvc_entropy_encoder> bvc_entropy_encoder_factory::create_entropy_encoder(bvc_entropy_coder in_coder)
{
	switch (in_coder)
	{
		case bvc_entropy_coder::BVC_ENTROPY_CODER_ARITHMETIC:
			return std::make_shared<arithmetic_encoder>();
		case bvc_entropy_coder::BVC_ENTROPY_CODER_HUFFMAN:
			return std::make_shared<huffman_encoder>();
		default:
			return nullptr;
	}
}