#pragma once

#include <memory>
#include <mutex>

#include "ovc_common/log/log.h"
#include "ovc_common/nal.h"
#include "ovc_common/picture.h"
#include "ovc_enc/config.h"
#include "ovc_enc/entropy/encoder_factory.h"
#include "ovc_enc/partition/partitioner_factory.h"
#include "ovc_enc/spiht/encoder_factory.h"
#include "ovc_enc/wavelet/decomposer_factory.h"
#include "ovc_enc/spiht/encoder.h"
#include "ovc_enc/result.h"

class ovc_encoder
{
public:
	ovc_encoder();

	ovc_enc_result init(ovc_enc_config* in_config);
	void		   set_logging_callback(ovc_logging_callback in_callback);
	ovc_enc_result encode(ovc_picture* in_picture, ovc_nal** out_nal_units, size_t* out_num_nal_units);

private:
	void encode_component(ovc_picture* in_picture, uint8_t in_component);
	void encode_partition(matrix<double> in_partition, uint16_t in_partition_id, uint8_t in_component);

	void construct_and_output_vps();

private:
	bool		   initialised;
	bool		   send_vps;
	ovc_enc_config config;

	std::mutex			 output_nals_mutex;
	std::vector<ovc_nal> output_nals;
};