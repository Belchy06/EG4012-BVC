#pragma once

#include <vector>

#include "ovc_common/bitstream/bitstream.h"
#include "ovc_common/math/matrix.h"
#include "ovc_common/spiht/spiht.h"
#include "ovc_dec/compress/decompressor.h"

class spiht_decompressor : public decompressor
{
public:
	spiht_decompressor();

	virtual void decompress(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, ovc::compression_config in_config) override;
	virtual void flush(matrix<double>& out_matrix) override;

private:
	void clear();
	void get_successor(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, int64_t* out_sx, int64_t* out_sy);

private:
	ovc::bitstream* bitstream;
	int				step;

	std::vector<ovc::spiht_pixel> lip;
	std::vector<ovc::spiht_pixel> lsp;
	std::vector<ovc::spiht_set>	  lis;
};