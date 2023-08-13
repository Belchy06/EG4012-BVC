#include "bvc_enc.h"

#include <vector>

bvc_encoder::bvc_encoder()
{
}

bvc_enc_result bvc_encoder::init(bvc_enc_config* in_config)
{
	// Validate config
	if (in_config->width == 0)
	{
		return bvc_enc_result::BVC_ENC_INVALID_DIMENSIONS;
	}

	if (in_config->height == 0)
	{
		return bvc_enc_result::BVC_ENC_INVALID_DIMENSIONS;
	}

	if (in_config->format == bvc_chroma_format::BVC_CHROMA_FORMAT_UNDEFINED)
	{
		return bvc_enc_result::BVC_ENC_INVALID_FORMAT;
	}

	config = *in_config;

	entropy_coder = bvc_entropy_encoder_factory::create_entropy_encoder(in_config->entropy_coder);

	return bvc_enc_result::BVC_ENC_OK;
}

bvc_enc_result bvc_encoder::encode(const uint8_t* in_picture_bytes, bvc_enc_nal** out_nal_units, int* out_num_nal_units)
{
	// TODO (belchy06): Actually compress lmao
	std::vector<bvc_enc_nal> output_nals;
	bvc_enc_nal				 nal;

	// Entropy coding
	int pic_bytes_size = get_size_in_bytes(config.format);

	if (entropy_coder != nullptr)
	{
		entropy_coder->encode(in_picture_bytes, pic_bytes_size);
		entropy_coder->flush(&nal.bytes, &nal.size);
	}
	else
	{
		nal.size = get_size_in_bytes(config.format);
		nal.bytes = new uint8_t[nal.size];
		memcpy(nal.bytes, in_picture_bytes, nal.size);
	}

	output_nals.push_back(nal);
	*out_nal_units = &output_nals[0];
	*out_num_nal_units = static_cast<int>(output_nals.size());

	return bvc_enc_result::BVC_ENC_OK;
}

int bvc_encoder::get_size_in_bytes(bvc_chroma_format in_format)
{
	int picture_samples = 0;
	if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_MONOCHROME)
	{
		picture_samples = config.width * config.height;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_420)
	{
		picture_samples = (3 * (config.width * config.height)) >> 1;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_422)
	{
		picture_samples = 2 * config.width * config.height;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_444)
	{
		picture_samples = 3 * config.width * config.height;
	}
	return picture_samples;
}