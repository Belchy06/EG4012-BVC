#include <cmath>

#include "ovc_dec/ovc_dec.h"
#include "ovc_dec/decoder.h"

#ifdef __cplusplus
extern "C"
{
#endif

	static ovc_dec_config* ovc_dec_config_create()
	{
		ovc_dec_config* config = new ovc_dec_config;
		std::memset(config, 0, sizeof(ovc_dec_config));
		return config;
	}

	static ovc_dec_result ovc_dec_config_destroy(ovc_dec_config* in_config)
	{
		if (in_config)
		{
			delete in_config;
		}
		return OVC_DEC_OK;
	}

	static ovc_dec_picture* ovc_dec_picture_create(ovc_decoder* in_encoder)
	{
		return new ovc_dec_picture();
	}

	static ovc_dec_result ovc_dec_picture_destroy(ovc_dec_picture* in_picture)
	{
		if (in_picture)
		{
			delete in_picture;
		}
		return OVC_DEC_OK;
	}

	static ovc_decoder* ovc_dec_decoder_create(ovc_dec_config* in_config)
	{
		ovc::decoder* decoder = new ovc::decoder();

		decoder->set_error_concealment(ovc::error_concealment(in_config->error_concealment));

		return decoder;
	}

	static ovc_dec_result ovc_dec_decoder_destroy(ovc_decoder* in_decoder)
	{
		if (in_decoder)
		{
			ovc::decoder* decoder = reinterpret_cast<ovc::decoder*>(in_decoder);
			delete decoder;
		}
		return OVC_DEC_OK;
	}

	static ovc_dec_result ovc_dec_decode_nal(ovc_decoder* in_decoder, ovc_dec_nal* in_nal)
	{
		if (!in_decoder || !in_nal)
		{
			return OVC_DEC_INVALID_ARGUMENT;
		}

		ovc::decoder* decoder = reinterpret_cast<ovc::decoder*>(in_decoder);
		bool		  success = decoder->decode_nal(reinterpret_cast<ovc::nal*>(in_nal));

		return success ? OVC_DEC_OK : OVC_DEC_ERROR;
	}

	static ovc_dec_result ovc_dec_flush(ovc_decoder* in_decoder)
	{
		if (!in_decoder)
		{
			return OVC_DEC_INVALID_ARGUMENT;
		}

		ovc::decoder* decoder = reinterpret_cast<ovc::decoder*>(in_decoder);
		bool		  success = decoder->flush();

		return success ? OVC_DEC_OK : OVC_DEC_ERROR;
	}

	static ovc_dec_result ovc_dec_get_picture(ovc_decoder* in_decoder, ovc_dec_picture* out_picture)
	{
		if (!in_decoder)
		{
			return OVC_DEC_INVALID_ARGUMENT;
		}

		ovc::decoder* decoder = reinterpret_cast<ovc::decoder*>(in_decoder);
		bool		  success = decoder->get_picture(reinterpret_cast<ovc::picture*>(out_picture));

		return success ? OVC_DEC_OK : OVC_DEC_NO_PICTURE;
	}

	static void ovc_dec_set_logging_callback(ovc_dec_logging_callback in_callback)
	{
		ovc::logging::logging_function = in_callback;
	}

	static void ovc_dec_set_logging_verbosity(ovc_dec_verbosity in_verbosity)
	{
		ovc::logging::verbosity = ovc::verbosity(in_verbosity);
	}

	static const ovc_decoder_api ovc_decoder_api_internal = {
		&ovc_dec_config_create,
		&ovc_dec_config_destroy,
		&ovc_dec_picture_create,
		&ovc_dec_picture_destroy,
		&ovc_dec_decoder_create,
		&ovc_dec_decoder_destroy,
		&ovc_dec_decode_nal,
		&ovc_dec_flush,
		&ovc_dec_get_picture,
		&ovc_dec_set_logging_callback,
		&ovc_dec_set_logging_verbosity
	};

	const ovc_decoder_api* ovc_decoder_api_get()
	{
		return &ovc_decoder_api_internal;
	}

#ifdef __cplusplus
} // extern "C"
#endif