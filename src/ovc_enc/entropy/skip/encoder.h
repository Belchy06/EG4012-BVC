#include "ovc_enc/entropy/encoder.h"

class skip_entropy_encoder : public ovc_entropy_encoder
{
public:
	virtual void encode(const uint8_t* in_bytes, size_t in_size) override;
	virtual void flush(uint8_t** out_bytes, size_t* out_size) override;
};