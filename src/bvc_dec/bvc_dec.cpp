#include "bvc_dec.h"

#include <cstring>

bvc_decoder::bvc_decoder()
{
}

bvc_dec_result bvc_decoder::init()
{
	entropy_decoder = bvc_entropy_decoder_factory::create_entropy_coder(bvc_entropy::BVC_ENTROPY_CODER_CABAC);

	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::decode_nal(bvc_dec_nal* in_nal_unit)
{
	if (entropy_decoder != nullptr)
	{
		for (int i = 0; i < in_nal_unit->size / 4; i++)
		{
			entropy_decoder->decode_symbol((uint32_t)(in_nal_unit->bytes[i] << 24 | in_nal_unit->bytes[i + 1] << 16 | in_nal_unit->bytes[i + 2] << 8 | in_nal_unit->bytes[i + 3] << 0));
		}

		uint32_t size = output_picture_bytes.size();
		entropy_decoder->flush(output_picture_bytes.data(), &size);
	}
	else
	{
		output_picture_bytes.resize(in_nal_unit->size);
		memcpy(output_picture_bytes.data(), in_nal_unit->bytes, in_nal_unit->size);
	}

	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::get_picture(bvc_decoded_picture* out_picture)
{
	out_picture->bytes = output_picture_bytes.empty() ? nullptr : output_picture_bytes.data();
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