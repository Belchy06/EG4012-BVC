#include "bvc_enc/entropy/encoder.h"

class skip_encoder : public bvc_entropy_encoder
{
public:
	virtual void encode(const uint8_t* in_bytes, size_t in_size) override;
	virtual void flush(uint8_t** out_bits, size_t* out_size) override;
};