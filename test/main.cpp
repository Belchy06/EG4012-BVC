#include <iostream>
#include <string>
#include <sstream>

#include "bvc_common/log.h"
#include "bvc_common/verbosity.h"
#include "decode/decode_test.h"
#include "e2e/e2e.h"
#include "entropy/entropy_test.h"
#include "encode/encode_test.h"
#include "partition/partition_test.h"
#include "wavelet/wavelet_test.h"

#define LogTest "LogTest"

int main(int argc, const char* argv[])
{
	bvc_logging::verbosity = BVC_VERBOSITY_SILENT;

	bool entropy = true;
	bool wavelet = true;
	bool partition = true;
	bool encode = true;
	bool decode = true;
	bool e2e = true;

	std::string source_path;
	std::string output_path;

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
                bvc_logging::verbosity = BVC_VERBOSITY_LOG;
            } else if(LevelStr == "verbose") {
                bvc_logging::verbosity = BVC_VERBOSITY_VERBOSE;
            } else if(LevelStr == "veryverbose") {
                bvc_logging::verbosity = BVC_VERBOSITY_VERY_VERBOSE;
            }
        } else if(arg == "-tests") {
            std::string TestStr(argv[++i]);
            
	        entropy = false;
	        wavelet = false;
	        partition = false;
	        encode = false;
            decode = false;
            e2e = false;

            std::stringstream ss(TestStr);
            std::string test;
            while(std::getline(ss, test, ','))
            {
                if(test == "entropy") {
                    entropy = true;
                } else if(test == "wavelet") {
                    wavelet = true;
                } else if(test == "partition") {
                    partition = true;
                } else if(test == "encode") {
                    encode = true;
                } else if(test == "decode") {
                    decode = true;
                } else if(test == "e2e") {
                    e2e = true;
                }
            }
        } else if(arg == "-source") {
            source_path = std::string(argv[++i]);
        } else if(arg == "-output") {
            output_path = std::string(argv[++i]);
        }
		// clang-format on
	}

	if (encode && output_path.empty())
	{
		std::exit(-1);
	}

	if (decode && output_path.empty())
	{
		std::exit(-1);
	}

	if (e2e && source_path.empty() && output_path.empty())
	{
		std::exit(-1);
	}

	bool bSuccess = true;
	if (entropy)
	{
		/**
		 * ENTROPY TEST
		 */
		// ARITHMETIC
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_ENTROPY_CODER_ARITHMETIC");
		bSuccess &= entropy_test::test(BVC_ENTROPY_CODER_ARITHMETIC, 32);
		bSuccess &= entropy_test::test(BVC_ENTROPY_CODER_ARITHMETIC, 64);
		bSuccess &= entropy_test::test(BVC_ENTROPY_CODER_ARITHMETIC, 128);

		// HUFFMAN
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_ENTROPY_CODER_HUFFMAN");
		bSuccess &= entropy_test::test(BVC_ENTROPY_CODER_HUFFMAN, 32);
		bSuccess &= entropy_test::test(BVC_ENTROPY_CODER_HUFFMAN, 64);
		bSuccess &= entropy_test::test(BVC_ENTROPY_CODER_HUFFMAN, 128);
	}

	if (wavelet)
	{
		/**
		 * WAVELET TEST
		 */
		// Haar
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_HAAR");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_HAAR, { .haar_config = { bvc_wavelet_haar_config::BVC_WAVELET_HAAR_DEFAULT } }, 64, 64, 4);
		// Bi-orthogonal

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_1p1");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_1p1 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_1p3");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_1p3 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_1p5");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_1p5 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p2");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p2 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p4");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p4 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p6");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p6 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_2p8");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_2p8 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p1");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p1 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p3");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p3 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p5");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p5 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p7");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p7 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_3p9");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_3p9 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_4p4");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_4p4 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_5p5");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_5p5 } }, 64, 64, 4);

		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_BIORTHOGONAL_6p8");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { bvc_wavelet_biorthogonal_config::BVC_WAVELET_BIORTHOGONAL_6p8 } }, 64, 64, 4);

		// Coiflets
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_1");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_1 } }, 64, 64, 4);
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_2");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_2 } }, 64, 64, 4);
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_3");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_3 } }, 64, 64, 4);
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_4");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_4 } }, 64, 64, 4);
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_WAVELET_COIFLETS_5");
		bSuccess &= wavelet_test::test(BVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { bvc_wavelet_coiflets_config::BVC_WAVELET_COIFLETS_5 } }, 64, 64, 4);
	}

	if (partition)
	{
		/**
		 * PARTITION TEST
		 */
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_PARTITION_OFFSET_ZEROTREE");
		bSuccess &= partition_test::test(BVC_PARTITION_OFFSET_ZEROTREE, 64, 64);
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_PARTITION_ZEROTREE_PRESERVING");
		bSuccess &= partition_test::test(BVC_PARTITION_ZEROTREE_PRESERVING, 64, 64);
	}

	if (encode)
	{
		/**
		 * ENCODE TEST
		 */
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_ENCODE");
		bSuccess &= encode_test::test(source_path, output_path);
	}

	if (decode)
	{
		/**
		 * DECODE TEST
		 */
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_DECODE");
		bSuccess &= decode_test::test(source_path, output_path);
	}

	if (e2e)
	{
		/**
		 * END2END TEST
		 */
		LOG(LogTest, BVC_VERBOSITY_VERBOSE, "BVC_E2E");
		bSuccess &= e2e_test::test(source_path, output_path);
	}
	std::cout << ((bSuccess) ? "Success" : "Failure") << std::endl;

	return bSuccess ? 0 : -1;
}