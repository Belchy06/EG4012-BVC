#include "bvc_dec.h"

#include <cstring>

bvc_decoder::bvc_decoder()
{
}

bvc_dec_result bvc_decoder::init()
{
	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::decode_nal(bvc_dec_nal* in_nal_unit)
{
	output_picture_bytes.resize(in_nal_unit->size);
	memcpy(output_picture_bytes.data(), in_nal_unit->bytes, in_nal_unit->size);

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