#include <iostream>

#include "bvc_common/verbosity.h"
#include "entropy/encoding_test.h"

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
	bSuccess = entropy_test::test(bvc_entropy::BVC_ENTROPY_CODER_ABAC, 32, verbosity);
	bSuccess = entropy_test::test(bvc_entropy::BVC_ENTROPY_CODER_ABAC, 64, verbosity);
	bSuccess = entropy_test::test(bvc_entropy::BVC_ENTROPY_CODER_ABAC, 128, verbosity);

	std::cout << ((bSuccess) ? "Success" : "Failure") << std::endl;

	return bSuccess ? 0 : -1;
}