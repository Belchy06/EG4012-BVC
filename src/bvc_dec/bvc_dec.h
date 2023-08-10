#pragma once

#include "picture.h"
#include "result.h"
#include "nal.h"

class bvc_decoder
{
public:
	bvc_decoder();

	bvc_dec_result init();
	bvc_dec_result decode_nal(bvc_dec_nal* in_nal_unit);
	bvc_dec_result get_picture(bvc_decoded_picture* out_picture);

private:
	bvc_decoded_picture* decoded_picture;
};