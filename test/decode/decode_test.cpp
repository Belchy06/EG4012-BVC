#include <memory>

#include "bvc_dec/bvc_dec.h"
#include "decode_test.h"
#include "../misc/lenna.h"

bool decode_test::test()
{
	std::shared_ptr<bvc_decoder> decoder = std::make_shared<bvc_decoder>();

	bvc_dec_result res;

	bvc_dec_config config{ 0 };

	res = decoder->init();

	// Construct image

	size_t*	 size_arr = new size_t[]{ 61, 63, 62, 62 };
	uint8_t* data1 = new uint8_t[]{ 0x1, 0xab, 0x2, 0xe2, 0xf0, 0x1c, 0xf8, 0xe3, 0x8b, 0x9a, 0x51, 0xeb, 0x43, 0xe, 0x38, 0x20, 0xf1, 0x8f, 0x76, 0x1e, 0xf0, 0xfc, 0x36, 0x13, 0x1a, 0x84, 0x8c, 0xcf, 0x52, 0xc6, 0xc9, 0x2b, 0x87, 0x47, 0x73, 0x60, 0x3d, 0x59, 0xf6, 0xdc, 0xeb, 0xec, 0x3, 0xbe, 0xe3, 0x33, 0xcb, 0x7b, 0x81, 0x4c, 0xf4, 0x6a, 0xe6, 0x87, 0xea, 0xcd, 0xcf, 0x1c, 0x5b, 0x7, 0x3d };
	uint8_t* data2 = new uint8_t[]{ 0x41, 0xa3, 0xab, 0x12, 0x33, 0x8, 0x39, 0x72, 0x54, 0x6e, 0x97, 0x48, 0x88, 0x9a, 0x51, 0xec, 0xdc, 0x93, 0xf4, 0xd9, 0x2d, 0x32, 0x9, 0x92, 0xce, 0x19, 0x89, 0x46, 0x7b, 0xc6, 0x33, 0x82, 0x57, 0xb8, 0xbc, 0x47, 0xef, 0xa0, 0x11, 0xa0, 0x17, 0x7a, 0xec, 0xb7, 0x36, 0xed, 0xaa, 0xf0, 0xc2, 0x16, 0xad, 0xf3, 0xc1, 0x24, 0xc1, 0x96, 0xd5, 0x7e, 0x10, 0xda, 0x35, 0x4d, 0x1 };
	uint8_t* data3 = new uint8_t[]{ 0x5, 0x5, 0xbd, 0x46, 0x4f, 0x33, 0x20, 0x67, 0x11, 0x90, 0x88, 0x8b, 0x7, 0x13, 0xcd, 0x56, 0xf, 0xfc, 0x18, 0x65, 0x9d, 0xb0, 0x8a, 0x7b, 0xc1, 0xa6, 0x5c, 0x33, 0xdd, 0x3b, 0x1, 0x4, 0x18, 0xfd, 0x60, 0x50, 0x9d, 0x5a, 0x4f, 0x8b, 0x3b, 0x42, 0x5d, 0x4e, 0x2a, 0x98, 0x5e, 0xcd, 0x78, 0x87, 0x6c, 0xd, 0x2a, 0xf6, 0xbb, 0x77, 0x9, 0x2, 0x52, 0xa8, 0x20, 0xb };
	uint8_t* data4 = new uint8_t[]{ 0x1, 0xab, 0xea, 0xd6, 0xda, 0xcf, 0x1d, 0x2, 0x66, 0x51, 0x5c, 0x38, 0x89, 0xc3, 0xbd, 0x3d, 0x68, 0x1d, 0x39, 0x87, 0x4c, 0x59, 0x7e, 0x6a, 0x65, 0x8e, 0xdd, 0x60, 0x5c, 0x83, 0x9, 0x73, 0x8e, 0xc3, 0xc9, 0x60, 0x9a, 0x48, 0x9f, 0x34, 0x24, 0x3e, 0x37, 0x29, 0x88, 0xca, 0xb6, 0x5d, 0x7, 0x3f, 0x1, 0xfb, 0xb1, 0x6e, 0x66, 0x44, 0x94, 0xd8, 0xef, 0x61, 0xb3, 0xdb };

	std::vector<uint8_t*> data;
	data.push_back(data1);
	data.push_back(data2);
	data.push_back(data3);
	data.push_back(data4);

	for (size_t i = 0; i < 4; i++)
	{
		bvc_nal nal;
		nal.bytes = data[i];
		nal.size = size_arr[i];

		res = decoder->decode_nal(&nal);
	}

	return res;
}
