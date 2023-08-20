#include "bvc_dec/bvc_dec.h"
#include "bvc_enc/bvc_enc.h"
#include "e2e.h"
#include "../util/ppm.h"
#include "../misc/lenna.h"

bool e2e_test::test(std::string in_source_path, std::string in_output_path)
{
	std::shared_ptr<bvc_encoder> encoder = std::make_shared<bvc_encoder>();

	bvc_enc_result enc_res;

	bvc_enc_config enc_config{ 0 };
	// Original image settings
	enc_config.format = BVC_CHROMA_FORMAT_MONOCHROME;
	enc_config.width = 64;
	enc_config.height = 64;

	// Wavelet transform settings
	enc_config.wavelet_family = BVC_WAVELET_FAMILY_BIORTHOGONAL;
	enc_config.wavelet_config = { .biorthogonal_config = BVC_WAVELET_BIORTHOGONAL_3p9 };

	// Partitioning settings
	enc_config.num_streams = 4;
	enc_config.partition_type = BVC_PARTITION_OFFSET_ZEROTREE;

	// SPIHT settings
	enc_config.bits_per_pixel = .01f;

	// Entropy coding settings
	enc_config.entropy_coder = BVC_ENTROPY_CODER_ARITHMETIC;

	enc_res = encoder->init(&enc_config);

	// Construct image
	bvc_picture input;
	// ppm::read(in_source_path, &input);
	input.Y = test::lenna_gray_64_raw;
	input.info.width = 64;
	input.info.height = 64;

	bvc_nal* nals;
	size_t	 num_nals;
	enc_res = encoder->encode(&input, &nals, &num_nals);

	std::shared_ptr<bvc_decoder> decoder = std::make_shared<bvc_decoder>();
	bvc_dec_result				 dec_res;

	bvc_dec_config dec_config{ 0 };

	dec_res = decoder->init();

	for (size_t i = 0; i < num_nals; i++)
	{
		dec_res = decoder->decode_nal(&nals[i]);
	}

	bvc_picture output;
	dec_res = decoder->get_picture(&output);

	ppm::write(in_output_path, &output, "P6");

	return true;
}