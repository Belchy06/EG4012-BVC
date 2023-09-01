#include "ovc_enc/compress/skip/compressor.h"

void skip_compressor::compress(matrix<double> in_matrix, ovc::compression_config in_config)
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

void skip_compressor::flush(uint8_t** out_bytes, uint16_t* out_step)
{
	*out_bytes = new uint8_t[bitstream->occupancy()];
	memcpy(*out_bytes, bitstream->data(), bitstream->occupancy());

	delete bitstream;
	bitstream = new ovc::bitstream();
}