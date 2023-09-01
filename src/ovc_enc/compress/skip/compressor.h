#pragma once

#include "ovc_enc/compress/compressor.h"

class skip_compressor : public compressor
{
public:
	virtual void compress(matrix<double> in_matrix, ovc::compression_config in_config) override;
	virtual void flush(uint8_t** out_bytes, uint16_t* out_step) override;
};