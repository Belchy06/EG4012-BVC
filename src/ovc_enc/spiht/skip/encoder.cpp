#include "ovc_enc/spiht/skip/encoder.h"

void skip_spiht_encoder::encode(matrix<double> in_matrix, ovc_spiht_config in_config)
{
	for (size_t j = 0; j < in_matrix.get_num_columns(); j++)
	{
		for (size_t i = 0; i < in_matrix.get_num_rows(); i++)
		{
			double	 val = in_matrix(j, i);
			uint8_t* val_ptr = reinterpret_cast<uint8_t*>(&val);
			bitstream->write_byte(val_ptr[7]);
			bitstream->write_byte(val_ptr[6]);
			bitstream->write_byte(val_ptr[5]);
			bitstream->write_byte(val_ptr[4]);
			bitstream->write_byte(val_ptr[3]);
			bitstream->write_byte(val_ptr[2]);
			bitstream->write_byte(val_ptr[1]);
			bitstream->write_byte(val_ptr[0]);
		}
	}
}

void skip_spiht_encoder::flush(uint8_t** out_bits, int* out_step)
{
	*out_bits = new uint8_t[bitstream->occupancy()];
	memcpy(*out_bits, bitstream->data(), bitstream->occupancy());

	delete bitstream;
	bitstream = new ovc_bitstream();
}