#pragma once

#include <vector>

#include "bvc_common/spiht/spiht.h"
#include "bvc_common/spiht/spiht_config.h"
#include "bvc_enc/spiht/encoder.h"

class spiht_encoder : public bvc_spiht_encoder
{
public:
	spiht_encoder();

	void encode(matrix<double> in_matrix, bvc_spiht_config in_config);
	void flush(uint8_t** out_bits, size_t* out_size, int* out_step);

private:
	void clear();
	void get_successor(matrix<double>& in_matrix, size_t in_num_levels, int in_x, int in_y, int* out_sx, int* out_sy);
	bool is_significant_pixel(matrix<double>& in_matrix, int in_x, int in_y);
	bool is_significant_set_A(matrix<double>& in_matrix, size_t in_num_levels, int in_x, int in_y, int in_count = 1);
	bool is_significant_set_B(matrix<double>& in_matrix, size_t in_num_levels, int in_x, int in_y, int in_count = 1);

private:
	size_t bits;
	int	   step;
	int	   output_step;

	std::vector<bvc_spiht_pixel> lip;
	std::vector<bvc_spiht_pixel> lsp;
	std::vector<bvc_spiht_set>	 lis;
};