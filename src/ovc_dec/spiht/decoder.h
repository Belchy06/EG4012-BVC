#pragma once

#include "ovc_common/bitstream/bitstream.h"
#include "ovc_common/math/matrix.h"
#include "ovc_common/spiht/spiht.h"

class ovc_spiht_decoder
{
public:
	virtual void decode(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, ovc_spiht_config in_config) = 0;
	virtual void flush(matrix<double>& out_matrix) = 0;

protected:
	matrix<double> output;
};