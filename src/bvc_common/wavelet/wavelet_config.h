#pragma once

#include "bvc_common/wavelet/biorthogonal/biorthogonal_config.h"
#include "bvc_common/wavelet/coiflets/coiflets_config.h"
#include "bvc_common/wavelet/haar/haar_config.h"
#include "bvc_common/wavelet/reverse_biorthogonal/reverse_biorthogonal_config.h"
#include "bvc_common/wavelet/symlets/symlets_config.h"

typedef union
{
	bvc_wavelet_biorthogonal_config			orthogonal_config;
	bvc_wavelet_coiflets_config				coiflets_config;
	bvc_wavelet_daubechies_config			daubechies_config;
	bvc_wavelet_haar_config					haar_config;
	bvc_wavelet_reverse_biorthogonal_config reverse_orthogonal_config;
	bvc_wavelet_symlets_config				symlets_config;
} bvc_wavelet_config;