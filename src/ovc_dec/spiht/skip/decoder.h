#pragma once

#include "ovc_dec/spiht/decoder.h"

class skip_spiht_decoder : public ovc_spiht_decoder
{
public:
	virtual void decode(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, ovc_spiht_config in_config) override;
	virtual void flush(matrix<double>& out_matrix) override;
};