#pragma once

#include <cmath>
#include <vector>

#include "bvc_common/wavelet/wavelet.h"

// haar
// https://wavelets.pybytes.com/wavelet/haar/
class haar : public bvc_wavelet_filters
{
private:
	static double inv_sqrt2;
};