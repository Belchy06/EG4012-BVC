#include "bvc_enc/entropy/abac/encoder.h"
#include "bvc_enc/entropy/huffman/encoder.h"

#include "bvc_enc/entropy/entropy_encoder_factory.h"

std::shared_ptr<bvc_entropy_encoder> bvc_entropy_encoder_factory::create_entropy_encoder(bvc_entropy_coder in_coder)
{
	switch (in_coder)
	{
		case bvc_entropy_coder::BVC_ENTROPY_CODER_ABAC:
			return std::make_shared<abac_encoder>();
		case bvc_entropy_coder::BVC_ENTROPY_CODER_HUFFMAN:
			return std::make_shared<huffman_encoder>();
		case bvc_entropy_coder::BVC_ENTROPY_CODER_NONE:
			return nullptr;
		default:
			return nullptr;
	}
}