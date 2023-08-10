#pragma once

#include <vector>

#include "nal.h"
#include "picture.h"
#include "result.h"

class bvc_decoder
{
public:
	bvc_decoder();

	bvc_dec_result init();
	bvc_dec_result decode_nal(bvc_dec_nal* in_nal_unit);
	bvc_dec_result get_picture(bvc_decoded_picture* out_picture);

private:
	int get_size_in_bytes(bvc_chroma_format in_format);

private:
	std::vector<uint8_t> output_picture_bytes;
};