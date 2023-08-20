#pragma once

#include "bvc_common/math/matrix.h"
#include "bvc_common/spiht/spiht.h"
#include "bvc_common/bitstream/bitstream.h"

class bvc_spiht_encoder
{
public:
	bvc_spiht_encoder()
		: bitstream(new bvc_bitstream()){};

	virtual void encode(matrix<double> in_matrix, bvc_spiht_config in_config) = 0;
	virtual void flush(uint8_t** out_bits, size_t* out_size, int* out_step) = 0;

protected:
	bvc_bitstream* bitstream;
};