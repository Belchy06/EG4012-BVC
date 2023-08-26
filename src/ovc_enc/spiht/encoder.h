#pragma once

#include "ovc_common/math/matrix.h"
#include "ovc_common/spiht/spiht.h"
#include "ovc_common/bitstream/bitstream.h"

class ovc_spiht_encoder
{
public:
	ovc_spiht_encoder()
		: bitstream(new ovc_bitstream()){};

	virtual void encode(matrix<double> in_matrix, ovc_spiht_config in_config) = 0;
	virtual void flush(uint8_t** out_bits, uint16_t* out_step) = 0;

protected:
	ovc_bitstream* bitstream;
};