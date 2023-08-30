#include <iostream>
#include <string>
#include <sstream>

#include "ovc_common/log/log.h"
#include "ovc_common/verbosity.h"
#include "decode/decode_test.h"
#include "e2e/e2e_test.h"
#include "entropy/entropy_test.h"
#include "encode/encode_test.h"
#include "interleave/interleave_test.h"
#include "partition/partition_test.h"
#include "resilience/resilience_test.h"
#include "wavelet/wavelet_test.h"

#define LogTest "LogTest"

int main(int argc, const char* argv[])
{
	ovc_logging::verbosity = OVC_VERBOSITY_SILENT;

	uint16_t seed = 1337;

	bool entropy = true;
	bool wavelet = true;
	bool partition = true;
	bool encode = true;
	bool decode = true;
	bool e2e = true;
	bool resilience = true;
	bool interleave = true;

	std::string source_path;
	std::string output_path;

	size_t num_dropped_nals;

	for (int i = 1; i < argc; i++)
	{
		std::string arg(argv[i]);

		// clang-format off
		if(argc - 1 == i) {
            std::cerr << "Error: Missing argument value: " << arg << std::endl;
			std::exit(1);
        } else if(arg == "-verbosity") {
            std::string LevelStr(argv[++i]);
            if(LevelStr == "silent") {
                ovc_logging::verbosity = OVC_VERBOSITY_SILENT;
            } else if(LevelStr == "error") {
                ovc_logging::verbosity = OVC_VERBOSITY_ERROR;
            } else if(LevelStr == "warning") {
                ovc_logging::verbosity = OVC_VERBOSITY_WARNING;
            } else if(LevelStr == "info") {
                ovc_logging::verbosity = OVC_VERBOSITY_INFO;
            } else if(LevelStr == "notice") {
                ovc_logging::verbosity = OVC_VERBOSITY_NOTICE;
            } else if(LevelStr == "verbose") {
                ovc_logging::verbosity = OVC_VERBOSITY_VERBOSE;
            } else if(LevelStr == "details") {
                ovc_logging::verbosity = OVC_VERBOSITY_DETAILS;
            }
        } else if(arg == "-tests") {
            std::string TestStr(argv[++i]);
            
	        entropy = false;
	        wavelet = false;
	        partition = false;
	        encode = false;
            decode = false;
            e2e = false;
            resilience = false;
            interleave = false;

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
                } else if(test == "resilience") {
                    resilience = true;
                } else if(test == "interleave") {
                    interleave = true;
                }
            }
        } else if(arg == "-source") {
            source_path = std::string(argv[++i]);
        } else if(arg == "-output") {
            output_path = std::string(argv[++i]);
        } else if(arg == "-num_dropped_nals") {
            std::stringstream(argv[++i]) >> num_dropped_nals;
        } else if(arg == "-seed") {
            std::stringstream(argv[++i]) >> seed;
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
		// SKIP
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_ENTROPY_CODER_SKIP");
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_SKIP, 32);
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_SKIP, 64);
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_SKIP, 128);

		// ARITHMETIC
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_ENTROPY_CODER_ARITHMETIC");
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_ARITHMETIC, 32);
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_ARITHMETIC, 64);
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_ARITHMETIC, 128);

		// HUFFMAN
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_ENTROPY_CODER_HUFFMAN");
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_HUFFMAN, 32);
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_HUFFMAN, 64);
		bSuccess &= entropy_test::test(OVC_ENTROPY_CODER_HUFFMAN, 128);
	}

	if (wavelet)
	{
		/**
		 * WAVELET TEST
		 */
		// Haar
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_HAAR");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_HAAR, { .haar_config = { ovc_wavelet_haar_config::OVC_WAVELET_HAAR_DEFAULT } }, 64, 64, 4);
		// Bi-orthogonal

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_1p1");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_1p1 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_1p3");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_1p3 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_1p5");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_1p5 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_2p2");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_2p2 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_2p4");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_2p4 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_2p6");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_2p6 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_2p8");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_2p8 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_3p1");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_3p1 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_3p3");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_3p3 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_3p5");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_3p5 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_3p7");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_3p7 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_3p9");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_3p9 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_4p4");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_4p4 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_5p5");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_5p5 } }, 64, 64, 4);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_BIORTHOGONAL_6p8");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_BIORTHOGONAL, { .biorthogonal_config = { ovc_wavelet_biorthogonal_config::OVC_WAVELET_BIORTHOGONAL_6p8 } }, 64, 64, 4);

		// Coiflets
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_COIFLETS_1");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { ovc_wavelet_coiflets_config::OVC_WAVELET_COIFLETS_1 } }, 64, 64, 4);
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_COIFLETS_2");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { ovc_wavelet_coiflets_config::OVC_WAVELET_COIFLETS_2 } }, 64, 64, 4);
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_COIFLETS_3");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { ovc_wavelet_coiflets_config::OVC_WAVELET_COIFLETS_3 } }, 64, 64, 4);
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_COIFLETS_4");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { ovc_wavelet_coiflets_config::OVC_WAVELET_COIFLETS_4 } }, 64, 64, 4);
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_WAVELET_COIFLETS_5");
		bSuccess &= wavelet_test::test(OVC_WAVELET_FAMILY_COIFLETS, { .coiflets_config = { ovc_wavelet_coiflets_config::OVC_WAVELET_COIFLETS_5 } }, 64, 64, 4);
	}

	if (partition)
	{
		/**
		 * PARTITION TEST
		 */
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_PARTITION_OFFSET_ZEROTREE");
		bSuccess &= partition_test::test(OVC_PARTITION_OFFSET_ZEROTREE, 64, 64);
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_PARTITION_ZEROTREE_PRESERVING");
		bSuccess &= partition_test::test(OVC_PARTITION_ZEROTREE_PRESERVING, 64, 64);
	}

	if (encode)
	{
		/**
		 * ENCODE TEST
		 */
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_ENCODE");
		bSuccess &= encode_test::test(source_path, output_path);
	}

	if (decode)
	{
		/**
		 * DECODE TEST
		 */
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_DECODE");
		bSuccess &= decode_test::test(source_path, output_path);
	}

	if (e2e)
	{
		/**
		 * END2END TEST
		 */
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_E2E");
		bSuccess &= e2e_test::test(source_path, output_path);
	}

	if (resilience)
	{
		/**
		 * RESILIENCE TEST
		 */
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_RESILIENCE");
		bSuccess &= resilience_test::test(num_dropped_nals, seed, output_path);
	}

	if (interleave)
	{
		/**
		 * INTERLEAVE TEST
		 */
		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_INTERLEAVE_SKIP");
		bSuccess &= interleave_test::test(OVC_INTERLEAVE_SKIP, seed, 32);

		OVC_LOG(LogTest, OVC_VERBOSITY_VERBOSE, "OVC_INTERLEAVE_RANDOM");
		bSuccess &= interleave_test::test(OVC_INTERLEAVE_RANDOM, seed, 32);
	}

	std::cout << ((bSuccess) ? "Success" : "Failure") << std::endl;

	return bSuccess ? 0 : -1;
}