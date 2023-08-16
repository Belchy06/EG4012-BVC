#pragma once

#include <vector>

#include "entropy/entropy_decoder.h"
#include "entropy/entropy_decoder_factory.h"
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
	uint8_t* output_picture_bytes;
	size_t	 output_picture_size;

	std::shared_ptr<bvc_entropy_decoder> entropy_decoder;
};