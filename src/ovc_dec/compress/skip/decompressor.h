#pragma once

#include "ovc_dec/compress/decompressor.h"

class skip_decompressor : public decompressor
{
public:
	virtual void decompress(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, ovc::compression_config in_config) override;
	virtual void flush(matrix<double>& out_matrix) override;
};