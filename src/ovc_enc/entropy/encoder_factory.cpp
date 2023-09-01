#include "ovc_enc/entropy/arithmetic/encoder.h"
#include "ovc_enc/entropy/encoder_factory.h"
#include "ovc_enc/entropy/huffman/encoder.h"
#include "ovc_enc/entropy/skip/encoder.h"

std::shared_ptr<entropy_encoder> entropy_encoder_factory::create_entropy_encoder(ovc::entropy_coder in_coder)
{
	switch (in_coder)
	{
		case ovc::ENTROPY_CODER_SKIP:
			return std::make_shared<skip_entropy_encoder>();
		case ovc::ENTROPY_CODER_ARITHMETIC:
			return std::make_shared<arithmetic_encoder>();
		case ovc::ENTROPY_CODER_HUFFMAN:
			return std::make_shared<huffman_encoder>();
		default:
			return nullptr;
	}
}