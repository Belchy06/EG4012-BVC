#pragma once

#include <map>

#include "ovc_common/nal.h"
#include "ovc_common/picture.h"
#include "ovc_dec/entropy/decoder_factory.h"
#include "ovc_dec/partition/departitioner_factory.h"
#include "ovc_dec/spiht/decoder_factory.h"
#include "ovc_dec/wavelet/recomposer_factory.h"
#include "ovc_dec/result.h"
#include "ovc_dec/config.h"

class ovc_decoder
{
public:
	ovc_decoder();

	ovc_dec_result init();
	ovc_dec_result decode_nal(const ovc_nal* in_nal_unit);
	ovc_dec_result get_picture(ovc_picture* out_picture);

private:
	int get_size_in_bytes(ovc_chroma_format in_format);

private:
	bool		picture_ready;
	ovc_picture picture;

	std::map<size_t, matrix<double>> partitions;

	std::shared_ptr<ovc_wavelet_recomposer> wavelet_recomposer;
	std::shared_ptr<ovc_departitioner>		departitioner;
	std::shared_ptr<ovc_spiht_decoder>		spiht_decoder;
	std::shared_ptr<ovc_entropy_decoder>	entropy_decoder;
};