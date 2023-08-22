#include "ovc_dec/ovc_dec.h"
#include "ovc_enc/ovc_enc.h"
#include "e2e.h"
#include "../util/ppm.h"
#include "../misc/lenna.h"

bool e2e_test::test(std::string in_source_path, std::string in_output_path)
{
	std::shared_ptr<ovc_encoder> encoder = std::make_shared<ovc_encoder>();

	ovc_enc_result enc_res;

	ovc_enc_config enc_config{ 0 };
	// Original image settings
	enc_config.format = OVC_CHROMA_FORMAT_MONOCHROME;
	enc_config.width = 64;
	enc_config.height = 64;

	// Wavelet transform settings
	enc_config.wavelet_family = OVC_WAVELET_FAMILY_BIORTHOGONAL;
	enc_config.wavelet_config = { .biorthogonal_config = OVC_WAVELET_BIORTHOGONAL_3p9 };

	// Partitioning settings
	enc_config.num_streams_exp = 1;
	enc_config.partition_type = OVC_PARTITION_OFFSET_ZEROTREE;

	// SPIHT settings
	enc_config.spiht = OVC_SPIHT_ENABLE;
	enc_config.bits_per_pixel = 5.f;

	// Entropy coding settings
	enc_config.entropy_coder = OVC_ENTROPY_CODER_ARITHMETIC;

	enc_res = encoder->init(&enc_config);

	// Construct image
	ovc_picture input;
	// ppm::read(in_source_path, &input);
	input.Y = test::lenna_gray_64_raw;
	input.info.width = 64;
	input.info.height = 64;

	ovc_nal* nals;
	size_t	 num_nals;
	enc_res = encoder->encode(&input, &nals, &num_nals);

	std::shared_ptr<ovc_decoder> decoder = std::make_shared<ovc_decoder>();
	ovc_dec_result				 dec_res;

	ovc_dec_config dec_config{ 0 };

	dec_res = decoder->init();

	for (size_t i = 0; i < num_nals; i++)
	{
		dec_res = decoder->decode_nal(&nals[i]);
	}

	ovc_picture output;
	dec_res = decoder->get_picture(&output);

	ppm::write(in_output_path, &output, "P6");

	return true;
}