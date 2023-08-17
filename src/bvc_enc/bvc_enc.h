#pragma once

#include "bvc_common/picture.h"
#include "bvc_enc/config.h"
#include "bvc_enc/entropy/entropy_encoder_factory.h"
#include "bvc_enc/partition/partitioner_factory.h"
#include "bvc_enc/wavelet/wavelet_decomposer_factory.h"
#include "bvc_enc/spiht/encoder.h"
#include "bvc_enc/result.h"
#include "bvc_enc/nal.h"

class bvc_encoder
{
public:
	bvc_encoder();

	bvc_enc_result init(bvc_enc_config* in_config);
	bvc_enc_result encode(bvc_picture* in_picture, bvc_enc_nal** out_nal_units, size_t* out_num_nal_units);

private:
	int get_size_in_bytes(bvc_chroma_format in_format);

private:
	bvc_enc_config config;

	std::shared_ptr<bvc_wavelet_decomposer> wavelet_decomposer;
	std::shared_ptr<bvc_partitioner>		partitioner;
	std::shared_ptr<bvc_spiht_encoder>		spiht_encoder;
	std::shared_ptr<bvc_entropy_encoder>	entropy_coder;
};