#pragma once

#include <vector>

#include "ovc_common/spiht/spiht.h"
#include "ovc_common/compress/compress_config.h"
#include "ovc_enc/compress/compressor.h"

class spiht_compressor : public compressor
{
public:
	spiht_compressor();

	void compress(matrix<double> in_matrix, ovc::compression_config in_config);
	void flush(uint8_t** out_bytes, uint16_t* out_step);

private:
	void clear();
	void get_successor(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, int64_t* out_sx, int64_t* out_sy);
	bool is_significant_pixel(matrix<double>& in_matrix, int64_t in_x, int64_t in_y, uint16_t in_step);
	bool is_significant_set_A(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, uint16_t in_step, int64_t in_count = 1);
	bool is_significant_set_B(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, uint16_t in_step, int64_t in_count = 1);

private:
	size_t	 bits;
	uint16_t output_step;

	std::vector<ovc::spiht_pixel> lip;
	std::vector<ovc::spiht_pixel> lsp;
	std::vector<ovc::spiht_set>	  lis;
};