#pragma once

#include "ovc_common/wavelet/biorthogonal/biorthogonal_config.h"
#include "ovc_common/wavelet/coiflets/coiflets_config.h"
#include "ovc_common/wavelet/daubechies/daubechies_config.h"
#include "ovc_common/wavelet/haar/haar_config.h"
#include "ovc_common/wavelet/reverse_biorthogonal/reverse_biorthogonal_config.h"
#include "ovc_common/wavelet/symlets/symlets_config.h"

namespace ovc
{
	typedef union
	{
		wavelet_biorthogonal_config			biorthogonal_config;
		wavelet_coiflets_config				coiflets_config;
		wavelet_daubechies_config			daubechies_config;
		wavelet_haar_config					haar_config;
		wavelet_reverse_biorthogonal_config reverse_biorthogonal_config;
		wavelet_symlets_config				symlets_config;
		uint8_t								value;
	} wavelet_config;
} // namespace ovc