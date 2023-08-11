#include "bvc_enc/entropy_coding/entropy_coder_factory.h"
#include "encoding_test.h"

bool entropy_test::test(bvc_entropy in_entropy)
{
	std::shared_ptr<bvc_entropy_encoder> entropy_coder = bvc_entropy_encoder_factory::create_entropy_encoder(in_entropy);
	return true;
}
