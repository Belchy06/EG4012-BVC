#include <bitset>
#include <memory>

#include "ovc_enc/ovc_enc.h"
#include "encode_test.h"
#include "../misc/lenna.h"

bool encode_test::test(std::string in_source_path, std::string in_output_path)
{
	std::shared_ptr<ovc_encoder> encoder = std::make_shared<ovc_encoder>();

	ovc_enc_result res;

	ovc_enc_config config{ 0 };
	// Original image settings
	config.format = OVC_CHROMA_FORMAT_MONOCHROME;
	config.width = 64;
	config.height = 64;

	// Wavelet transform settings
	config.wavelet_family = OVC_WAVELET_FAMILY_BIORTHOGONAL;
	config.wavelet_config = { .biorthogonal_config = OVC_WAVELET_BIORTHOGONAL_3p9 };

	// Partitioning settings
	config.num_streams_exp = 1;
	config.partition_type = OVC_PARTITION_OFFSET_ZEROTREE;

	// SPIHT settings
	config.bits_per_pixel = 0.5f;

	// Entropy coding settings
	config.entropy_coder = OVC_ENTROPY_CODER_ARITHMETIC;

	res = encoder->init(&config);

	// Construct image
	ovc_picture picture;
	picture.planes[0].data = test::lenna_gray_64_raw;

	ovc_nal* nals;
	size_t	 num_nals;
	res = encoder->encode(&picture, &nals, &num_nals);

	return res == OVC_ENC_OK;
}
