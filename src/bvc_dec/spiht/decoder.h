#pragma once

#include <vector>

#include "bvc_common/bitstream/bitstream.h"
#include "bvc_common/math/matrix.h"
#include "bvc_common/spiht/spiht.h"
#include "bvc_common/spiht/spiht_config.h"

class bvc_spiht_decoder
{
public:
	bvc_spiht_decoder();

	void decode(uint8_t* in_bits, size_t in_num_bytes, size_t in_x, size_t in_y, size_t in_num_levels, bvc_spiht_config in_config);
	void flush(matrix<double>& out_matrix);

private:
	void clear();
	void get_successor(matrix<double>& in_matrix, size_t in_num_levels, int in_x, int in_y, int* out_sx, int* out_sy);

private:
	matrix<double> output;
	bvc_bitstream* bitstream;
	int			   step;

	std::vector<bvc_spiht_pixel> lip;
	std::vector<bvc_spiht_pixel> lsp;
	std::vector<bvc_spiht_set>	 lis;
};