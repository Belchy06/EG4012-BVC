#include <iostream>

#include "entropy/encoding_test.h"

int main(int argc, const char* argv[])
{
	bool bSuccess = true;
	bSuccess = entropy_test::test(bvc_entropy::BVC_ENTROPY_CODER_CABAC);

	std::cout << ((bSuccess) ? "Success" : "Failure") << std::endl;

	return bSuccess ? 0 : -1;
}