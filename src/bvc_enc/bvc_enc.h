#pragma once

#include "config.h"
#include "result.h"
#include "nal.h"

class bvc_encoder
{
public:
	bvc_encoder();

	bvc_enc_result init(bvc_enc_config* in_config);
	bvc_enc_result encode(const uint8_t* in_picture_bytes, bvc_enc_nal** out_nal_units, int* out_num_nal_units);

private:
	int get_size_in_bytes(bvc_chroma_format in_format);

private:
	bvc_enc_config config;
};