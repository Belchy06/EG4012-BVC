#include "ovc_dec/spiht/spiht/decoder.h"

#include <iostream>

spiht_decoder::spiht_decoder()
	: bitstream(new ovc_bitstream())
{
	clear();
}

void spiht_decoder::decode(uint8_t* in_bytes, size_t in_num_bytes, size_t in_x, size_t in_y, ovc_spiht_config in_config)
{
	for (size_t i = 0; i < in_num_bytes; i++)
	{
		bitstream->write_byte(in_bytes[i]);
	}

	output = matrix<double>(in_y, in_x);
	size_t width = in_x;
	size_t height = in_y;
	// TODO (belchy06): This should update based on the plane
	size_t bit_allocation = (size_t)ceil(width * height * in_config.bpp);

	step = in_config.step;

	for (size_t y = 0; y <= height / (1 << in_config.num_levels); y++)
	{
		for (size_t x = 0; x <= width / (1 << in_config.num_levels); x++)
		{
			lip.push_back(ovc_spiht_pixel(x, y));
			if ((x % 2 != 0) || (y % 2 != 0))
			{
				lis.push_back(ovc_spiht_set(x, y, OVC_SPIHT_TYPE_A));
			}
		}
	}

	size_t bit_cnt = 0;
	while (step >= 0)
	{
		// sorting pass
		// process LIP
		for (int64_t i = 0; i < lip.size(); i++)
		{
			uint8_t sig = 0;
			bitstream->read_bit(&sig);
			if (++bit_cnt > bit_allocation)
			{
				return;
			}
			if (sig)
			{
				lsp.push_back(ovc_spiht_pixel(lip[i].x, lip[i].y));
				uint8_t bit = 0;
				bitstream->read_bit(&bit);
				output(lip[i].y, lip[i].x) = (float)((((bool)bit) ? -1 : 1) * (1 << step));
				if (++bit_cnt > bit_allocation)
				{
					return;
				}
				lip.erase(lip.begin() + i);
				i--;
			}
		}
		// process LIS
		for (int64_t i = 0; i < lis.size(); i++)
		{
			if (lis[i].type == OVC_SPIHT_TYPE_A)
			{
				uint8_t sig = 0;
				bitstream->read_bit(&sig);
				if (++bit_cnt > bit_allocation)
				{
					return;
				}
				if (sig)
				{
					int64_t sx, sy;
					get_successor(output, in_config.num_levels, lis[i].x, lis[i].y, &sx, &sy);
					/* process the four offsprings */
					sig = 0;
					bitstream->read_bit(&sig);
					if (++bit_cnt > bit_allocation)
					{
						return;
					}
					if (sig)
					{
						lsp.push_back(ovc_spiht_pixel(sx, sy));
						uint8_t bit = 0;
						bitstream->read_bit(&bit);
						output(sy, sx) = (float)((((bool)bit) ? -1 : 1) * (1 << step));
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(ovc_spiht_pixel(sx, sy));
					}
					sig = 0;
					bitstream->read_bit(&sig);
					if (++bit_cnt > bit_allocation)
					{
						return;
					}
					if (sig)
					{
						lsp.push_back(ovc_spiht_pixel(sx + 1, sy));
						uint8_t bit = 0;
						bitstream->read_bit(&bit);
						output(sy, sx + 1) = (float)((((bool)bit) ? -1 : 1) * (1 << step));
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(ovc_spiht_pixel(sx + 1, sy));
					}
					sig = 0;
					bitstream->read_bit(&sig);
					if (++bit_cnt > bit_allocation)
					{
						return;
					}
					if (sig)
					{
						lsp.push_back(ovc_spiht_pixel(sx, sy + 1));
						uint8_t bit = 0;
						bitstream->read_bit(&bit);
						output(sy + 1, sx) = (float)((((bool)bit) ? -1 : 1) * (1 << step));
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(ovc_spiht_pixel(sx, sy + 1));
					}
					sig = 0;
					bitstream->read_bit(&sig);
					if (++bit_cnt > bit_allocation)
						return;
					if (sig)
					{
						lsp.push_back(ovc_spiht_pixel(sx + 1, sy + 1));
						uint8_t bit = 0;
						bitstream->read_bit(&bit);
						output(sy + 1, sx + 1) = (float)((((bool)bit) ? -1 : 1) * (1 << step));
						if (++bit_cnt > bit_allocation)
						{
							return;
						}
					}
					else
					{
						lip.push_back(ovc_spiht_pixel(sx + 1, sy + 1));
					}
					/* test if L(i, j) != 0 */
					get_successor(output, in_config.num_levels, sx, sy, &sx, &sy);
					if (sx != -1)
					{
						lis.push_back(ovc_spiht_set(lis[i].x, lis[i].y, OVC_SPIHT_TYPE_B));
					}
					lis.erase(lis.begin() + i);
					i--;
				}
			}
			else
			{
				uint8_t sig = 0;
				bitstream->read_bit(&sig);
				if (++bit_cnt > bit_allocation)
				{
					return;
				}
				if (sig)
				{
					int64_t sx, sy;
					get_successor(output, in_config.num_levels, lis[i].x, lis[i].y, &sx, &sy);
					lis.push_back(ovc_spiht_set(sx, sy, OVC_SPIHT_TYPE_A));
					lis.push_back(ovc_spiht_set(sx + 1, sy, OVC_SPIHT_TYPE_A));
					lis.push_back(ovc_spiht_set(sx, sy + 1, OVC_SPIHT_TYPE_A));
					lis.push_back(ovc_spiht_set(sx + 1, sy + 1, OVC_SPIHT_TYPE_A));
					lis.erase(lis.begin() + i);
					i--;
				}
			}
		}
		// Refinement pass
		for (int64_t i = 0; i < lsp.size(); i++)
		{
			if (std::abs((int64_t)output(lsp[i].y, lsp[i].x)) >= (1 << (step + 1)))
			{
				uint8_t bit = 0;
				bitstream->read_bit(&bit);
				if (bit)
				{
					if ((int64_t)output(lsp[i].y, lsp[i].x) >= 0)
						output(lsp[i].y, lsp[i].x) = (float)(((int64_t)output(lsp[i].y, lsp[i].x)) | (1 << step));
					else
						output(lsp[i].y, lsp[i].x) = (float)(-(((int64_t)std::abs((int64_t)output(lsp[i].y, lsp[i].x))) | (1 << step)));
				}
				else
				{
					output(lsp[i].y, lsp[i].x) = (float)(((int64_t)output(lsp[i].y, lsp[i].x)) & (~(1 << step)));
				}
				if (++bit_cnt > bit_allocation)
				{
					return;
				}
			}
		}
		// update quantization step
		step--;
		std::cout << output << std::endl
				  << std::endl;
	}
}

void spiht_decoder::flush(matrix<double>& out_matrix)
{
	out_matrix = output;
	clear();
}

void spiht_decoder::clear()
{
	delete bitstream;
	lip.clear();
	lsp.clear();
	lis.clear();

	step = 0;
	bitstream = new ovc_bitstream();
}

void spiht_decoder::get_successor(matrix<double>& in_matrix, size_t in_num_levels, int64_t in_x, int64_t in_y, int64_t* out_sx, int64_t* out_sy)
{
	int64_t lx = (in_matrix.get_num_columns()) / (1 << in_num_levels);
	int64_t ly = (in_matrix.get_num_rows()) / (1 << in_num_levels);
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