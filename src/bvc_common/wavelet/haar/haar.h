#pragma once

#include <cmath>
#include <vector>

#include "bvc_common/wavelet/wavelet.h"

// haar
// https://wavelets.pybytes.com/wavelet/haar/
class haar
{
public:
	static std::vector<double> lo_d;
	static std::vector<double> hi_d;
	static std::vector<double> lo_r;
	static std::vector<double> hi_r;

private:
	static double inv_sqrt2;
};