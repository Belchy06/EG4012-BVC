#pragma once

#include <vector>

#include "bvc_common/math/matrix.h"
#include "bvc_common/spiht/spiht.h"
#include "bvc_common/spiht/spiht_config.h"
#include "bvc_dec/spiht/decoder.h"

class spiht_decoder : public bvc_spiht_decoder
{
public:
	spiht_decoder();

	virtual void decode(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, bvc_spiht_config in_config) override;
	virtual void flush(matrix<double>& out_matrix) override;

private:
	void clear();
	void get_successor(matrix<double>& in_matrix, size_t in_num_levels, int in_x, int in_y, int* out_sx, int* out_sy);

private:
	bvc_bitstream* bitstream;
	int			   step;

	std::vector<bvc_spiht_pixel> lip;
	std::vector<bvc_spiht_pixel> lsp;
	std::vector<bvc_spiht_set>	 lis;
};