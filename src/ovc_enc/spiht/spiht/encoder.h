#pragma once

#include <vector>

#include "ovc_common/spiht/spiht.h"
#include "ovc_common/spiht/spiht_config.h"
#include "ovc_enc/spiht/encoder.h"

class spiht_encoder : public ovc_spiht_encoder
{
public:
	spiht_encoder();

	void encode(matrix<double> in_matrix, ovc_spiht_config in_config);
	void flush(uint8_t** out_bits, int* out_step);

private:
	void clear();
	void get_successor(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, int64_t* out_sx, int64_t* out_sy);
	bool is_significant_pixel(matrix<double>& in_matrix, int64_t in_x, int64_t in_y);
	bool is_significant_set_A(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, int64_t in_count = 1);
	bool is_significant_set_B(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, int64_t in_count = 1);

private:
	size_t bits;
	int	   step;
	int	   output_step;

	std::vector<ovc_spiht_pixel> lip;
	std::vector<ovc_spiht_pixel> lsp;
	std::vector<ovc_spiht_set>	 lis;
};