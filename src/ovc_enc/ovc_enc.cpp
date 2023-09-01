#include <cmath>

#include "ovc_enc/ovc_enc.h"
#include "ovc_enc/encoder.h"

#ifdef __cplusplus
extern "C"
{
#endif

	static ovc_enc_config* ovc_enc_config_create()
	{
		ovc_enc_config* config = new ovc_enc_config;
		std::memset(config, 0, sizeof(ovc_enc_config));
		return config;
	}

	static ovc_enc_result ovc_enc_config_destroy(ovc_enc_config* in_config)
	{
		if (in_config)
		{
			delete in_config;
		}
		return OVC_ENC_OK;
	}

	static ovc_enc_result ovc_enc_config_validate(ovc_enc_config* in_config)
	{
		// Validate config
		if (in_config->width == 0)
		{
			return OVC_ENC_INVALID_DIMENSIONS;
		}

		if (in_config->height == 0)
		{
			return OVC_ENC_INVALID_DIMENSIONS;
		}

		if (in_config->format == ovc_enc_chroma_format::OVC_ENC_CHROMA_FORMAT_UNDEFINED)
		{
			return OVC_ENC_INVALID_FORMAT;
		}

		if ((in_config->num_levels > 1 || in_config->num_parts_exp > 0) && in_config->partitioner == ovc_enc_partitioner::OVC_ENC_PARTITIONER_SKIP)
		{
			return OVC_ENC_INVALID_PARAMETER;
		}

		if ((in_config->num_levels != -1) && (in_config->num_parts_exp != -1))
		{
			if (pow(4, in_config->num_parts_exp) > in_config->width * in_config->height / pow(4, in_config->num_levels))
			{
				return OVC_ENC_INVALID_PARAMETER;
			}
		}

		if ((in_config->num_parts_exp == -1) && (in_config->num_levels == -1))
		{
			// Both params are unset, calculate heuristically
			if (in_config->preset == OVC_ENC_PRESET_RESILLIENCE)
			{
				in_config->num_levels = 0;
				in_config->num_parts_exp = int(log(in_config->width * in_config->height / pow(4, in_config->num_levels)) / log(4));
			}
			else if (in_config->preset == OVC_ENC_PRESET_COMPRESSION)
			{
				int num_levels_x = 0;
				while (in_config->width % (1 << (num_levels_x + 1)) == 0)
				{
					num_levels_x++;
				}

				int num_levels_y = 0;
				while (in_config->height % (1 << (num_levels_y + 1)) == 0)
				{
					num_levels_y++;
				}

				in_config->num_parts_exp = (num_levels_x < num_levels_y) ? num_levels_x : num_levels_y;
			}
			else if (in_config->preset == OVC_ENC_PRESET_BALANCED)
			{
				int num_levels_x = 0;
				while (in_config->width % (1 << (num_levels_x + 1)) == 0)
				{
					num_levels_x++;
				}

				int num_levels_y = 0;
				while (in_config->height % (1 << (num_levels_y + 1)) == 0)
				{
					num_levels_y++;
				}

				in_config->num_levels = int(floor(((num_levels_x < num_levels_y) ? num_levels_x : num_levels_y) / 2));
				in_config->num_parts_exp = int(floor((log(in_config->width * in_config->height / pow(4, in_config->num_levels)) / log(4)) / 2));
			}
		}

		// Calculate levels from partitions or vice-versa. If user has specified both, ensure that the values are compatible
		if ((in_config->num_levels != -1) && (in_config->num_parts_exp == -1))
		{
			// User has specified number of wavelet levels. Calculate number of streams accordingly
			if (in_config->preset == OVC_ENC_PRESET_RESILLIENCE)
			{
				in_config->num_parts_exp = int(log(in_config->width * in_config->height / pow(4, in_config->num_levels)) / log(4));
			}
			else if (in_config->preset == OVC_ENC_PRESET_COMPRESSION)
			{
				in_config->num_parts_exp = 0;
			}
			else if (in_config->preset == OVC_ENC_PRESET_BALANCED)
			{
				in_config->num_parts_exp = int(floor((log(in_config->width * in_config->height / pow(4, in_config->num_levels)) / log(4)) / 2));
			}
		}
		else if ((in_config->num_parts_exp != -1) && (in_config->num_levels == -1))
		{
			int num_levels_x = 0;
			while (in_config->width % (1 << (num_levels_x + 1)) == 0)
			{
				num_levels_x++;
			}

			int num_levels_y = 0;
			while (in_config->height % (1 << (num_levels_y + 1)) == 0)
			{
				num_levels_y++;
			}

			int max_image_levels = (num_levels_x < num_levels_y) ? num_levels_x : num_levels_y;
			int max_part_levels = int(floor(log(in_config->width * in_config->height / pow(4, in_config->num_parts_exp)) / log(4)));

			in_config->num_levels = (max_part_levels < max_image_levels) ? max_part_levels : max_image_levels;
		}

		return OVC_ENC_OK;
	}

	static ovc_enc_picture* ovc_enc_picture_create(ovc_encoder* in_encoder)
	{
		return new ovc_enc_picture();
	}

	static ovc_enc_result ovc_enc_picture_destroy(ovc_enc_picture* in_picture)
	{
		if (in_picture)
		{
			delete in_picture;
		}
		return OVC_ENC_OK;
	}

	static ovc_encoder* ovc_enc_encoder_create(ovc_enc_config* in_config)
	{
		if (ovc_enc_config_validate(in_config) != OVC_ENC_OK)
		{
			return nullptr;
		}

		ovc::encoder* encoder = new ovc::encoder();

		encoder->set_width(in_config->width);
		encoder->set_height(in_config->height);
		encoder->set_format(ovc::chroma_format(in_config->format));
		encoder->set_framerate(in_config->framerate);
		encoder->set_bits_per_pixel(in_config->bits_per_pixel);
		encoder->set_seed(in_config->seed);
		encoder->set_num_parts_exp(in_config->num_parts_exp);
		encoder->set_num_levels(in_config->num_levels);
		encoder->set_wavelet_family(ovc::wavelet_family(in_config->wavelet_family));
		encoder->set_wavelet_config({ .value = in_config->wavelet_config.value });
		encoder->set_partitioner(ovc::partition(in_config->partitioner));
		encoder->set_compressor(ovc::compression(in_config->compressor));
		encoder->set_entropy_coder(ovc::entropy_coder(in_config->entropy_coder));
		encoder->set_interleaver(ovc::interleave(in_config->interleaver));

		return encoder;
	}
	static ovc_enc_result ovc_enc_encoder_destroy(ovc_encoder* in_encoder)
	{
		if (in_encoder)
		{
			ovc::encoder* encoder = reinterpret_cast<ovc::encoder*>(in_encoder);
			delete encoder;
		}
		return OVC_ENC_OK;
	}
	static ovc_enc_result ovc_enc_encode(ovc_encoder* in_encoder, ovc_enc_picture* in_picture, ovc_enc_nal** out_nals, size_t* out_num_nals)
	{
		if (!in_encoder || !in_picture || !out_nals || !out_num_nals)
		{
			return OVC_ENC_INVALID_ARGUMENT;
		}

		ovc::encoder* encoder = reinterpret_cast<ovc::encoder*>(in_encoder);
		bool		  success = encoder->encode(reinterpret_cast<ovc::picture*>(in_picture));
		if (!success)
		{
			*out_nals = nullptr;
			*out_num_nals = 0;
			return OVC_ENC_ERROR;
		}

		std::vector<ovc_enc_nal> output_nals;
		for (auto& nal : encoder->get_output_nals())
		{
			output_nals.push_back(*reinterpret_cast<ovc_enc_nal*>(&nal));
		}
		if (output_nals.size() > 0)
		{
			*out_nals = &output_nals[0];
			*out_num_nals = static_cast<size_t>(output_nals.size());
		}
		else
		{
			*out_nals = nullptr;
			*out_num_nals = 0;
		}

		return OVC_ENC_OK;
	}

	static void ovc_enc_set_logging_callback(ovc_enc_logging_callback in_callback)
	{
		ovc::logging::logging_function = in_callback;
	}

	static void ovc_enc_set_logging_verbosity(ovc_enc_verbosity in_verbosity)
	{
		ovc::logging::verbosity = ovc::verbosity(in_verbosity);
	}

	static const ovc_encoder_api ovc_encoder_api_internal = {
		&ovc_enc_config_create,
		&ovc_enc_config_destroy,
		&ovc_enc_config_validate,
		&ovc_enc_picture_create,
		&ovc_enc_picture_destroy,
		&ovc_enc_encoder_create,
		&ovc_enc_encoder_destroy,
		&ovc_enc_encode,
		&ovc_enc_set_logging_callback,
		&ovc_enc_set_logging_verbosity
	};

	const ovc_encoder_api* ovc_encoder_api_get()
	{
		return &ovc_encoder_api_internal;
	}

#ifdef __cplusplus
} // extern "C"
#endif