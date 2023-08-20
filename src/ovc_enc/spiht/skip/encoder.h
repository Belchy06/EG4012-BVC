#pragma once

#include "ovc_enc/spiht/encoder.h"

class skip_spiht_encoder : public ovc_spiht_encoder
{
public:
	virtual void encode(matrix<double> in_matrix, ovc_spiht_config in_config) override;
	virtual void flush(uint8_t** out_bits, size_t* out_size, int* out_step) override;
};