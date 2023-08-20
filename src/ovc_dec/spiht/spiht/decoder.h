#pragma once

#include <vector>

#include "ovc_common/math/matrix.h"
#include "ovc_common/spiht/spiht.h"
#include "ovc_common/spiht/spiht_config.h"
#include "ovc_dec/spiht/decoder.h"

class spiht_decoder : public ovc_spiht_decoder
{
public:
	spiht_decoder();

	virtual void decode(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, ovc_spiht_config in_config) override;
	virtual void flush(matrix<double>& out_matrix) override;

private:
	void clear();
	void get_successor(matrix<double>& in_matrix, size_t in_num_levels, int in_x, int in_y, int* out_sx, int* out_sy);

private:
	ovc_bitstream* bitstream;
	int			   step;

	std::vector<ovc_spiht_pixel> lip;
	std::vector<ovc_spiht_pixel> lsp;
	std::vector<ovc_spiht_set>	 lis;
};