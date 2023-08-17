#include "bvc_enc/spiht/spiht_encoder.h"

#include <cmath>

bvc_spiht_encoder::bvc_spiht_encoder()
{
	clear();
}

void bvc_spiht_encoder::encode(matrix<double> in_matrix, size_t in_num_stages, bvc_spiht_config in_config)
{
	clear();

	size_t width = in_matrix.get_num_columns();
	size_t height = in_matrix.get_num_rows();
	// TODO (belchy06): This should update based on the component
	size_t bit_allocation = (size_t)ceil(width * height * in_config.bpp);

	int max = INT_MIN;
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			double pixel = in_matrix(x, y);
			if (std::abs(pixel) > max)
			{
				max = (int)std::abs(pixel);
			}
		}
	}

	step = (int)floor(log((float)max) / log(2.f));
	for (size_t y = 0; y < height / (1 << in_num_stages); y++)
	{
		for (size_t x = 0; x < width / (1 << in_num_stages); x++)
		{
			lip.push_back(bvc_spiht_pixel(x, y));
			if ((x % 2 != 0) || (y % 2 != 0))
			{
				lis.push_back(bvc_spiht_set(x, y, BVC_SPIHT_TYPE_A));
			}
		}
	}

	size_t bit_cnt = 0;
	while (step >= 0)
	{
		// Sorting pass
		// First process LIP
		for (size_t i = 0; i < lip.size(); i++)
		{
			bool significant = is_significant_pixel(in_matrix, lip[i].x, lip[i].y);
			bitstream->write_bit(uint8_t(significant));
			if (++bit_cnt > bit_allocation)
			{
				return;
			}
			if (significant)
			{
				lsp.push_back(bvc_spiht_pixel(lip[i].x, lip[i].y));
				bitstream->write_bit(((int)in_matrix(lip[i].x, lip[i].y)) > 0 ? 0 : 1);
				if (++bit_cnt > bit_allocation)
				{
					return;
				}
				lip.erase(lip.begin() + i);
				i--;
			}
		}
		// now process LIS
		for (size_t i = 0; i < lis.size(); i++)
		{
			if (lis[i].type == BVC_SPIHT_TYPE_A)
			{
				bool significant = is_significant_set_A(in_matrix, in_num_stages, lis[i].x, lis[i].y);
				bitstream->write_bit((uint8_t)significant);
				if (++bit_cnt > bit_allocation)
				{
					return;
				}
				if (significant)
				{
					int sx, sy;
					get_successor(in_matrix, in_num_stages, lis[i].x, lis[i].y, &sx, &sy);
					/* process the four offsprings */
					significant = is_significant_pixel(in_matrix, sx, sy);
					bitstream->write_bit((uint8_t)significant);
					if (++bit_cnt > bit_allocation)
					{
						return;
					}

					if (significant)
					{
						lsp.push_back(bvc_spiht_pixel(sx, sy));
						bitstream->write_bit(((int)in_matrix(sx, sy)) > 0 ? 0 : 1);
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(bvc_spiht_pixel(sx, sy));
					}
					significant = is_significant_pixel(in_matrix, sx + 1, sy);
					bitstream->write_bit((uint8_t)significant);
					if (++bit_cnt > bit_allocation)
					{
						return;
					}
					if (significant)
					{
						lsp.push_back(bvc_spiht_pixel(sx + 1, sy));
						bitstream->write_bit(((int)in_matrix(sx + 1, sy)) > 0 ? 0 : 1);
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(bvc_spiht_pixel(sx + 1, sy));
					}
					significant = is_significant_pixel(in_matrix, sx, sy + 1);
					bitstream->write_bit((uint8_t)significant);
					if (++bit_cnt > bit_allocation)
					{
						return;
					}
					if (significant)
					{
						lsp.push_back(bvc_spiht_pixel(sx, sy + 1));
						bitstream->write_bit(((int)in_matrix(sx, sy + 1)) > 0 ? 0 : 1);
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(bvc_spiht_pixel(sx, sy + 1));
					}
					significant = is_significant_pixel(in_matrix, sx + 1, sy + 1);
					bitstream->write_bit((uint8_t)significant);
					if (++bit_cnt > bit_allocation)
					{
						return;
					}
					if (significant)
					{
						lsp.push_back(bvc_spiht_pixel(sx + 1, sy + 1));
						bitstream->write_bit(((int)in_matrix(sx + 1, sy + 1)) > 0 ? 0 : 1);
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(bvc_spiht_pixel(sx + 1, sy + 1));
					}
					/* test if L(i, j) != 0 */
					get_successor(in_matrix, in_num_stages, sx, sy, &sx, &sy);
					if (sx != -1)
						lis.push_back(bvc_spiht_set(lis[i].x, lis[i].y, BVC_SPIHT_TYPE_B));
					lis.erase(lis.begin() + i);
					i--;
				}
			}
			else
			{
				bool significant = is_significant_set_B(in_matrix, in_num_stages, lis[i].x, lis[i].y);
				bitstream->write_bit((uint8_t)significant);
				if (++bit_cnt > bit_allocation)
				{
					return;
				}
				if (significant)
				{
					int sx, sy;
					get_successor(in_matrix, in_num_stages, lis[i].x, lis[i].y, &sx, &sy);
					lis.push_back(bvc_spiht_set(sx, sy, BVC_SPIHT_TYPE_A));
					lis.push_back(bvc_spiht_set(sx + 1, sy, BVC_SPIHT_TYPE_A));
					lis.push_back(bvc_spiht_set(sx, sy + 1, BVC_SPIHT_TYPE_A));
					lis.push_back(bvc_spiht_set(sx + 1, sy + 1, BVC_SPIHT_TYPE_A));
					lis.erase(lis.begin() + i);
					i--;
				}
			}
		}
	}
}

void bvc_spiht_encoder::flush(uint8_t** out_bits, size_t* out_size)
{
}

void bvc_spiht_encoder::clear()
{
	lip.clear();
	lsp.clear();
	lis.clear();

	step = 0;
}

void bvc_spiht_encoder::get_successor(matrix<double>& in_matrix, size_t in_num_stages, int in_x, int in_y, int* out_sx, int* out_sy)
{
	int lx = (in_matrix.get_num_columns()) / (1 << in_num_stages);
	int ly = (in_matrix.get_num_rows()) / (1 << in_num_stages);
	if (in_x < lx && in_y < ly)
	{
		if (in_x % 2 == 1)
			*out_sx = in_x + lx - 1;
		else
			*out_sx = in_x;
		if (in_y % 2 == 1)
			*out_sy = in_y + ly - 1;
		else
			*out_sy = in_y;
		if (*out_sx == in_x && *out_sy == in_y)
		{
			*out_sx = -1;
			*out_sy = -1;
		}
	}
	else
	{
		*out_sx = 2 * in_x;
		*out_sy = 2 * in_y;
		if (*out_sx >= (in_matrix.get_num_columns()) || *out_sy >= (in_matrix.get_num_rows()))
		{
			*out_sx = -1;
			*out_sy = -1;
		}
	}
}

bool bvc_spiht_encoder::is_significant_pixel(matrix<double>& in_matrix, int in_x, int in_y)
{
	return std::abs((int)in_matrix(in_x, in_y)) >= (1 << step);
}

bool bvc_spiht_encoder::is_significant_set_A(matrix<double>& in_matrix, size_t in_num_stages, int in_x, int in_y, int in_count)
{
	if (in_count > 1 && is_significant_pixel(in_matrix, in_x, in_y))
		return true;
	int sx, sy;
	get_successor(in_matrix, in_num_stages, in_x, in_y, &sx, &sy);
	if (sx == -1 || sy == -1)
		return false;
	if (is_significant_set_A(in_matrix, sx, sy, in_count + 1))
		return true;
	else if (is_significant_set_A(in_matrix, sx + 1, sy, in_count + 1))
		return true;
	else if (is_significant_set_A(in_matrix, sx, sy + 1, in_count + 1))
		return true;
	else if (is_significant_set_A(in_matrix, sx + 1, sy + 1, in_count + 1))
		return true;
	return false;
}

bool bvc_spiht_encoder::is_significant_set_B(matrix<double>& in_matrix, size_t in_num_stages, int in_x, int in_y, int in_count)
{
	if (in_count > 2 && is_significant_pixel(in_matrix, in_x, in_y))
		return true;
	int sx, sy;
	get_successor(in_matrix, in_num_stages, in_x, in_y, &sx, &sy);
	if (sx == -1 || sy == -1)
		return false;
	if (is_significant_set_B(in_matrix, sx, sy, in_count + 1))
		return true;
	else if (is_significant_set_B(in_matrix, sx + 1, sy, in_count + 1))
		return true;
	else if (is_significant_set_B(in_matrix, sx, sy + 1, in_count + 1))
		return true;
	else if (is_significant_set_B(in_matrix, sx + 1, sy + 1, in_count + 1))
		return true;
	return false;
}
