#pragma once

#include "ovc_common/math/matrix.h"
#include "ovc_common/compress/compress_config.h"
#include "ovc_common/bitstream/bitstream.h"

class compressor
{
public:
	compressor()
		: bitstream(new ovc::bitstream()){};

	virtual void compress(matrix<double> in_matrix, ovc::compression_config in_config) = 0;
	virtual void flush(uint8_t** out_bytes, uint16_t* out_step) = 0;

protected:
	ovc::bitstream* bitstream;
};