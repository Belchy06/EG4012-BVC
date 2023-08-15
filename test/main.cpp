#include <iostream>

#include "bvc_common/verbosity.h"
#include "entropy/entropy_test.h"
#include "wavelet/wavelet_test.h"

#define LOG(level, message)                    \
	{                                          \
		if (verbosity >= level)                \
		{                                      \
			std::cout << message << std::endl; \
		}                                      \
	}

int main(int argc, const char* argv[])
{
	bvc_verbosity verbosity = BVC_VERBOSITY_SILENT;
	for (int i = 1; i < argc; i++)
	{
		std::string arg(argv[i]);

		// clang-format off
		if(argc - 1 == i) {
            std::cerr << "Error: Missing argument value: " << arg << std::endl;
			std::exit(1);
        } else if(arg == "-verbosity") {
            std::string LevelStr(argv[++i]);
            if(LevelStr == "log") {
                verbosity = BVC_VERBOSITY_LOG;
            } else if(LevelStr == "verbose") {
                verbosity = BVC_VERBOSITY_VERBOSE;
            } else if(LevelStr == "veryverbose") {
                verbosity = BVC_VERBOSITY_VERY_VERBOSE;
            }
        }
		// clang-format on
	}

	bool bSuccess = true;
	/**
	 * ENTROPY TEST
	 */
	// ABAC
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_ENTROPY_CODER_ABAC");
	bSuccess = entropy_test::test(bvc_entropy_coder::BVC_ENTROPY_CODER_ABAC, 32, verbosity);
	bSuccess = entropy_test::test(bvc_entropy_coder::BVC_ENTROPY_CODER_ABAC, 64, verbosity);
	bSuccess = entropy_test::test(bvc_entropy_coder::BVC_ENTROPY_CODER_ABAC, 128, verbosity);
	// HUFFMAN
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_ENTROPY_CODER_HUFFMAN");
	bSuccess = entropy_test::test(bvc_entropy_coder::BVC_ENTROPY_CODER_HUFFMAN, 32, verbosity);
	bSuccess = entropy_test::test(bvc_entropy_coder::BVC_ENTROPY_CODER_HUFFMAN, 64, verbosity);
	bSuccess = entropy_test::test(bvc_entropy_coder::BVC_ENTROPY_CODER_HUFFMAN, 128, verbosity);

	/**
	 * WAVELET TEST
	 */
	// Haar
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_HAAR");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_HAAR, { .haar_config = { bvc_wavelet_haar_config::BVC_WAVELET_HAAR_DEFAULT } }, 64, 64, 4, verbosity);
	// Bi-orthogonal
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_1p1");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_1p1 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_1p3");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_1p3 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_1p5");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_1p5 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p2");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p2 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p4");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p4 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p6");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p6 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p8");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p8 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p1");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p1 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p3");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p3 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p5");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p5 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p7");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p7 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p9");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p9 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_4p4");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_4p4 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_5p5");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_5p5 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_6p8");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_6p8 } }, 64, 64, 4, verbosity);
	// Coiflets
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_1");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_1 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_2");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_2 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_3");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_3 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_4");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_4 } }, 64, 64, 4, verbosity);
	LOG(BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_5");
	bSuccess = wavelet_test::test(bvc_wavelet::BVC_WAVELET_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_5 } }, 64, 64, 4, verbosity);

	std::cout << ((bSuccess) ? "Success" : "Failure") << std::endl;

	return bSuccess ? 0 : -1;
}