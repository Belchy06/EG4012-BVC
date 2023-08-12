#include "bvc_dec.h"

#include <cstring>

bvc_decoder::bvc_decoder()
{
}

bvc_dec_result bvc_decoder::init()
{
	entropy_decoder = bvc_entropy_decoder_factory::create_entropy_decoder(bvc_entropy::BVC_ENTROPY_CODER_CABAC);

	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::decode_nal(bvc_dec_nal* in_nal_unit)
{
	if (entropy_decoder != nullptr)
	{

		// TODO: Parse nal header for raw byte length

		// Parse first 4 bytes out of nal unit for num symbols
		// TODO (belchy06): Using 16bits for num symbols is excessive (num symbols in bytes)
		uint32_t num_bytes = (uint32_t)(in_nal_unit->bytes[1] << 8 | in_nal_unit->bytes[0] << 0);
		for (size_t i = 0; i < num_bytes; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				// Only read data out of the nal if it's there, otherwise feed in 1's
				uint8_t bit = (i < in_nal_unit->size) ? (in_nal_unit->bytes[i + 2] >> j) & 0x1 : 1;
				entropy_decoder->decode(bit);
			}
		}

		output_picture_bytes = new uint8_t();
		entropy_decoder->flush(&output_picture_bytes, &output_picture_size);
	}
	else
	{
		output_picture_size = in_nal_unit->size;
		output_picture_bytes = new uint8_t[output_picture_size];
		memcpy(output_picture_bytes, in_nal_unit->bytes, output_picture_size);
	}

	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::get_picture(bvc_decoded_picture* out_picture)
{
	out_picture->bytes = output_picture_bytes;
	// TODO (belchy06): Parse headers
	out_picture->info.framerate = 29.97f;
	out_picture->info.width = 176;
	out_picture->info.height = 144;
	out_picture->info.bit_depth = 8;
	out_picture->info.format = bvc_chroma_format::BVC_CHROMA_FORMAT_420;
	out_picture->size = get_size_in_bytes(out_picture->info.format);

	return bvc_dec_result::BVC_DEC_OK;
}

int bvc_decoder::get_size_in_bytes(bvc_chroma_format in_format)
{
	int picture_samples = 0;
	if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_MONOCHROME)
	{
		picture_samples = 176 * 144;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_420)
	{
		picture_samples = (3 * (176 * 144)) >> 1;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_422)
	{
		picture_samples = 2 * 176 * 144;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_444)
	{
		picture_samples = 3 * 176 * 144;
	}
	return picture_samples;
}