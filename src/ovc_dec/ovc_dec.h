#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OVC_DEC_API
	#define OVC_DEC_API
#endif

	typedef enum
	{
		OVC_DEC_OK,
		OVC_DEC_ERROR,
		OVC_DEC_UNINITIALISED,
		OVC_DEC_NO_PICTURE,
		OVC_DEC_MALFORMED_NAL_HEADER,
		OVC_DEC_MALFORMED_NAL_BODY,
		OVC_DEC_MISSING_VPS,
		OVC_DEC_MISSING_PPS,
		OVC_DEC_INVALID_ARGUMENT
	} ovc_dec_result;

	typedef enum
	{
		OVC_DEC_CHROMA_FORMAT_UNDEFINED,
		OVC_DEC_CHROMA_FORMAT_MONOCHROME,
		OVC_DEC_CHROMA_FORMAT_420,
		OVC_DEC_CHROMA_FORMAT_422,
		OVC_DEC_CHROMA_FORMAT_444,
	} ovc_dec_chroma_format;

	typedef enum
	{
		OVC_DEC_VERBOSITY_SILENT,
		OVC_DEC_VERBOSITY_ERROR,
		OVC_DEC_VERBOSITY_WARNING,
		OVC_DEC_VERBOSITY_INFO,
		OVC_DEC_VERBOSITY_NOTICE,
		OVC_DEC_VERBOSITY_VERBOSE,
		OVC_DEC_VERBOSITY_DETAILS
	} ovc_dec_verbosity;

	typedef enum
	{
		OVC_DEC_ERROR_CONCEALMENT_SKIP,
		OVC_DEC_ERROR_CONCEALMENT_AVERAGE_RECEIVED,
		OVC_DEC_ERROR_CONCEALMENT_AVERAGE_SURROUNDING
	} ovc_dec_error_concealment;

	typedef struct ovc_dec_config
	{
		ovc_dec_error_concealment error_concealment;
	} ovc_dec_config;

	typedef struct ovc_dec_nal
	{
	public:
		uint8_t* bytes;
		size_t	 size;
	} ovc_dec_nal;

	typedef struct ovc_decoder ovc_decoder;

	typedef struct ovc_dec_plane
	{
		uint8_t* data;
		size_t	 width;
		size_t	 height;
		size_t	 bit_depth;
	} ovc_dec_plane;

	typedef struct ovc_dec_picture
	{
		ovc_dec_plane		  planes[3];
		ovc_dec_chroma_format format;
		double				  framerate;
	} ovc_dec_picture;

	typedef void (*ovc_dec_logging_callback)(int, const char*, va_list);

	typedef struct ovc_decoder_api
	{
		// config
		ovc_dec_config* (*config_create)(void);
		ovc_dec_result (*config_destroy)(ovc_dec_config* in_config);
		// picture
		ovc_dec_picture* (*picture_create)(ovc_decoder* in_decoder);
		ovc_dec_result (*picture_destroy)(ovc_dec_picture* in_picture);
		// decoder
		ovc_decoder* (*decoder_create)(ovc_dec_config* in_config);
		ovc_dec_result (*decoder_destroy)(ovc_decoder* in_decoder);
		ovc_dec_result (*decode_nal)(ovc_decoder* in_decoder, ovc_dec_nal* in_nal);
		ovc_dec_result (*flush)(ovc_decoder* in_decoder);
		ovc_dec_result (*get_picture)(ovc_decoder* in_decoder, ovc_dec_picture* out_picture);
		// misc
		void (*set_logging_callback)(ovc_dec_logging_callback in_callback);
		void (*set_logging_verbosity)(ovc_dec_verbosity in_verbosity);
	} ovc_decoder_api;

	OVC_DEC_API const ovc_decoder_api* ovc_decoder_api_get(void);

#ifdef __cplusplus
} // extern "C"
#endif
