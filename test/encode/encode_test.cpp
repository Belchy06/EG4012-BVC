#include <bitset>
#include <memory>

#include "bvc_enc/bvc_enc.h"
#include "encode_test.h"
#include "../misc/lenna.h"

bool encode_test::test()
{
	std::shared_ptr<bvc_encoder> encoder = std::make_shared<bvc_encoder>();

	bvc_enc_result res;

	bvc_enc_config config{ 0 };
	// Original image settings
	config.format = BVC_CHROMA_FORMAT_MONOCHROME;
	config.width = 64;
	config.height = 64;

	// Wavelet transform settings
	config.wavelet_family = BVC_WAVELET_BIORTHOGONAL;
	config.wavelet_config = { .biorthogonal_config = BVC_WAVELET_BIORTHOGONAL_3p9 };

	// Partitioning settings
	config.num_streams = 4;
	config.partition_type = BVC_PARTITION_OFFSET_ZEROTREE;

	// SPIHT settings
	config.bits_per_pixel = 0.5f;

	// Entropy coding settings
	config.entropy_coder = BVC_ENTROPY_CODER_ARITHMETIC;

	res = encoder->init(&config);

	// Construct image
	bvc_picture picture;
	picture.Y = test::lenna_gray_64_raw;

	bvc_nal* nals;
	size_t	 num_nals;
	res = encoder->encode(&picture, &nals, &num_nals);

	for (size_t i = 0; i < num_nals; i++)
	{
		uint8_t* nal_data = nals[i].bytes;
		size_t	 nal_size = nals[i].size;

		// std::string raw_string = "";
		for (size_t i = 0; i < nal_size; i++)
		{
			// std::bitset<8> x(nal_data[i]);
			int t = nal_data[i];
			std::cout << "0x" << std::hex << t << std::dec << ", ";
		}
		std::cout << std::endl;
	}

	return res == BVC_ENC_OK;
}
