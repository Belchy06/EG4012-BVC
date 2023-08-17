#include "bvc_enc.h"

#include <iostream>
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

	// Calculate levels from streams or vice-versa. If user has specified both, ensure that the values are compatible
	if (in_config->num_levels && !in_config->num_streams)
	{
		in_config->num_streams = (size_t)(in_config->width * in_config->height / pow(4, in_config->num_levels));
	}
	else if (in_config->num_streams && !in_config->num_levels)
	{
		in_config->num_levels = (size_t)(log2(in_config->width * in_config->height / in_config->num_streams) / log2(4));
	}
	else if (in_config->num_levels && in_config->num_streams)
	{
		if (in_config->num_streams != in_config->width * in_config->height / pow(4, in_config->num_levels))
		{
			return bvc_enc_result::BVC_ENC_INVALID_PARAM;
		}
	}

	config = *in_config;

	wavelet_decomposer = bvc_wavelet_decomposer_factory::create_wavelet_decomposer(in_config->wavelet_family, in_config->wavelet_config);
	partitioner = bvc_partitioner_factory::create_partitioner(in_config->partition_type);
	spiht_encoder = std::make_shared<bvc_spiht_encoder>();
	entropy_coder = bvc_entropy_encoder_factory::create_entropy_encoder(in_config->entropy_coder);

	return bvc_enc_result::BVC_ENC_OK;
}

bvc_enc_result bvc_encoder::encode(bvc_picture* in_picture, bvc_enc_nal** out_nal_units, size_t* out_num_nal_units)
{
	std::vector<bvc_enc_nal> output_nals;

	// TODO (belchy06): Add support for U and V planes
	matrix<double>		Y(config.width, config.height);
	std::vector<double> y_data;
	for (size_t x = 0; x < config.width; x++)
	{
		for (size_t y = 0; y < config.height; y++)
		{
			y_data.push_back((double)in_picture->Y[x + y * config.width]);
		}
	}

	Y.set_data(y_data);

	bvc_wavelet_decomposition_2d<double> decomposition = wavelet_decomposer->decompose(Y, config.num_levels);
	matrix<double>						 decomp_matrix = decomposition.get_matrix();

	std::vector<matrix<double>> streams = partitioner->partition(decomp_matrix, config.num_levels);
	// TODO (belchy06): Parallelize
	for (matrix<double> stream : streams)
	{
		spiht_encoder->encode(stream, config.num_levels, { .bpp = config.bits_per_pixel });
		uint8_t* spiht_bitstream = new uint8_t();
		size_t	 spiht_byte_length = 0;
		spiht_encoder->flush(&spiht_bitstream, &spiht_byte_length);

		entropy_coder->encode(spiht_bitstream, spiht_byte_length);
		uint8_t* entropy_bitstream = new uint8_t();
		size_t	 entropy_byte_length = 0;
		entropy_coder->flush(&entropy_bitstream, &entropy_byte_length);

		bvc_enc_nal nal;
		nal.bytes = entropy_bitstream;
		nal.size = entropy_byte_length;
		output_nals.push_back(nal);
	}

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