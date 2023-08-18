#pragma once

#include <map>
#include <vector>

#include "entropy/entropy_decoder_factory.h"
#include "wavelet/wavelet_recomposer_factory.h"
#include "partition/departitioner_factory.h"
#include "spiht/decoder.h"
#include "bvc_common/nal.h"
#include "bvc_common/picture.h"
#include "result.h"
#include "config.h"

class bvc_decoder
{
public:
	bvc_decoder();

	bvc_dec_result init();
	bvc_dec_result decode_nal(const bvc_nal* in_nal_unit);
	bvc_dec_result get_picture(bvc_picture* out_picture);

private:
	int get_size_in_bytes(bvc_chroma_format in_format);

private:
	bool		picture_ready;
	bvc_picture picture;

	std::map<size_t, matrix<double>> partitions;

	std::shared_ptr<bvc_wavelet_recomposer> wavelet_recomposer;
	std::shared_ptr<bvc_departitioner>		departitioner;
	std::shared_ptr<bvc_spiht_decoder>		spiht_decoder;
	std::shared_ptr<bvc_entropy_decoder>	entropy_decoder;
};