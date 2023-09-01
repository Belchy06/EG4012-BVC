#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OVC_ENC_API
	#define OVC_ENC_API
#endif

	typedef enum
	{
		OVC_ENC_OK,
		OVC_ENC_UNINITIALISED,
		OVC_ENC_INVALID_DIMENSIONS,
		OVC_ENC_INVALID_FORMAT,
		OVC_ENC_INVALID_PARAMETER,
		OVC_ENC_INVALID_ARGUMENT,
		OVC_ENC_ERROR
	} ovc_enc_result;

	typedef enum
	{
		OVC_ENC_PRESET_RESILLIENCE,
		OVC_ENC_PRESET_COMPRESSION,
		OVC_ENC_PRESET_BALANCED
	} ovc_enc_preset;

	typedef enum
	{
		OVC_ENC_CHROMA_FORMAT_UNDEFINED,
		OVC_ENC_CHROMA_FORMAT_MONOCHROME,
		OVC_ENC_CHROMA_FORMAT_420,
		OVC_ENC_CHROMA_FORMAT_422,
		OVC_ENC_CHROMA_FORMAT_444,
	} ovc_enc_chroma_format;

	typedef enum
	{
		OVC_ENC_WAVELET_FAMILY_SKIP,
		OVC_ENC_WAVELET_FAMILY_BIORTHOGONAL,
		OVC_ENC_WAVELET_FAMILY_COIFLETS,
		OVC_ENC_WAVELET_FAMILY_DAUBECHIES,
		OVC_ENC_WAVELET_FAMILY_HAAR,
		OVC_ENC_WAVELET_FAMILY_REVERSE_BIORTHOGONAL,
		OVC_ENC_WAVELET_FAMILY_SYMLETS
	} ovc_enc_wavelet_family;

	typedef enum : uint8_t
	{
		OVC_ENC_WAVELET_BIORTHOGONAL_1p1,
		OVC_ENC_WAVELET_BIORTHOGONAL_1p3,
		OVC_ENC_WAVELET_BIORTHOGONAL_1p5,
		OVC_ENC_WAVELET_BIORTHOGONAL_2p2,
		OVC_ENC_WAVELET_BIORTHOGONAL_2p4,
		OVC_ENC_WAVELET_BIORTHOGONAL_2p6,
		OVC_ENC_WAVELET_BIORTHOGONAL_2p8,
		OVC_ENC_WAVELET_BIORTHOGONAL_3p1,
		OVC_ENC_WAVELET_BIORTHOGONAL_3p3,
		OVC_ENC_WAVELET_BIORTHOGONAL_3p5,
		OVC_ENC_WAVELET_BIORTHOGONAL_3p7,
		OVC_ENC_WAVELET_BIORTHOGONAL_3p9,
		OVC_ENC_WAVELET_BIORTHOGONAL_4p4,
		OVC_ENC_WAVELET_BIORTHOGONAL_5p5,
		OVC_ENC_WAVELET_BIORTHOGONAL_6p8,
	} ovc_enc_wavelet_biorthogonal_config;

	typedef enum : uint8_t
	{
		OVC_ENC_WAVELET_COIFLETS_1,
		OVC_ENC_WAVELET_COIFLETS_2,
		OVC_ENC_WAVELET_COIFLETS_3,
		OVC_ENC_WAVELET_COIFLETS_4,
		OVC_ENC_WAVELET_COIFLETS_5
	} ovc_enc_wavelet_coiflets_config;

	typedef enum : uint8_t
	{
		OVC_ENC_WAVELET_DAUBECHIES_1,
		OVC_ENC_WAVELET_DAUBECHIES_2,
		OVC_ENC_WAVELET_DAUBECHIES_3,
		OVC_ENC_WAVELET_DAUBECHIES_4,
		OVC_ENC_WAVELET_DAUBECHIES_5,
		OVC_ENC_WAVELET_DAUBECHIES_6,
		OVC_ENC_WAVELET_DAUBECHIES_7,
		OVC_ENC_WAVELET_DAUBECHIES_8,
		OVC_ENC_WAVELET_DAUBECHIES_9,
		OVC_ENC_WAVELET_DAUBECHIES_10,
		OVC_ENC_WAVELET_DAUBECHIES_11,
		OVC_ENC_WAVELET_DAUBECHIES_12,
		OVC_ENC_WAVELET_DAUBECHIES_13,
		OVC_ENC_WAVELET_DAUBECHIES_14,
		OVC_ENC_WAVELET_DAUBECHIES_15,
		OVC_ENC_WAVELET_DAUBECHIES_16,
		OVC_ENC_WAVELET_DAUBECHIES_17,
		OVC_ENC_WAVELET_DAUBECHIES_18,
		OVC_ENC_WAVELET_DAUBECHIES_19,
		OVC_ENC_WAVELET_DAUBECHIES_20
	} ovc_enc_wavelet_daubechies_config;

	typedef enum : uint8_t
	{
		OVC_ENC_WAVELET_HAAR_DEFAULT
	} ovc_enc_wavelet_haar_config;

	typedef enum : uint8_t
	{
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_1p1,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_1p3,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_1p5,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_2p2,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_2p4,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_2p6,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_2p8,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_3p1,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_3p3,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_3p5,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_3p7,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_3p9,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_4p4,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_5p5,
		OVC_ENC_WAVELET_REVERSE_BIORTHOGONAL_6p8
	} ovc_enc_wavelet_reverse_biorthogonal_config;

	typedef enum : uint8_t
	{
		OVC_ENC_WAVELET_SYMLETS_2,
		OVC_ENC_WAVELET_SYMLETS_3,
		OVC_ENC_WAVELET_SYMLETS_4,
		OVC_ENC_WAVELET_SYMLETS_5,
		OVC_ENC_WAVELET_SYMLETS_6,
		OVC_ENC_WAVELET_SYMLETS_7,
		OVC_ENC_WAVELET_SYMLETS_8,
		OVC_ENC_WAVELET_SYMLETS_9,
		OVC_ENC_WAVELET_SYMLETS_10,
		OVC_ENC_WAVELET_SYMLETS_11,
		OVC_ENC_WAVELET_SYMLETS_12,
		OVC_ENC_WAVELET_SYMLETS_13,
		OVC_ENC_WAVELET_SYMLETS_14,
		OVC_ENC_WAVELET_SYMLETS_15,
		OVC_ENC_WAVELET_SYMLETS_16,
		OVC_ENC_WAVELET_SYMLETS_17,
		OVC_ENC_WAVELET_SYMLETS_18,
		OVC_ENC_WAVELET_SYMLETS_19,
		OVC_ENC_WAVELET_SYMLETS_20
	} ovc_enc_wavelet_symlets_config;

	typedef union
	{
		ovc_enc_wavelet_biorthogonal_config			biorthogonal_config;
		ovc_enc_wavelet_coiflets_config				coiflets_config;
		ovc_enc_wavelet_daubechies_config			daubechies_config;
		ovc_enc_wavelet_haar_config					haar_config;
		ovc_enc_wavelet_reverse_biorthogonal_config reverse_biorthogonal_config;
		ovc_enc_wavelet_symlets_config				symlets_config;
		uint8_t										value;
	} ovc_enc_wavelet_config;

	typedef enum
	{
		OVC_ENC_PARTITIONER_SKIP,
		OVC_ENC_PARTITIONER_OFFSET_ZEROTREE,
		OVC_ENC_PARTITIONER_ZEROTREE_PRESERVING
	} ovc_enc_partitioner;

	typedef enum
	{
		OVC_ENC_COMPRESSOR_SKIP,
		OVC_ENC_COMPRESSOR_SPIHT
	} ovc_enc_compressor;

	typedef enum
	{
		OVC_ENC_ENTROPY_CODER_SKIP,
		OVC_ENC_ENTROPY_CODER_ARITHMETIC,
		OVC_ENC_ENTROPY_CODER_HUFFMAN
	} ovc_enc_entropy_coder;

	typedef enum
	{
		OVC_ENC_INTERLEAVER_SKIP,
		OVC_ENC_INTERLEAVER_RANDOM
	} ovc_enc_interleaver;

	typedef enum
	{
		OVC_ENC_VERBOSITY_SILENT,
		OVC_ENC_VERBOSITY_ERROR,
		OVC_ENC_VERBOSITY_WARNING,
		OVC_ENC_VERBOSITY_INFO,
		OVC_ENC_VERBOSITY_NOTICE,
		OVC_ENC_VERBOSITY_VERBOSE,
		OVC_ENC_VERBOSITY_DETAILS
	} ovc_enc_verbosity;

	typedef struct ovc_enc_config
	{
		size_t				  width;
		size_t				  height;
		ovc_enc_chroma_format format;
		float				  framerate;
		float				  bits_per_pixel;
		uint16_t			  seed;

		int num_parts_exp = -1;
		int num_levels = -1;

		ovc_enc_preset		   preset;
		ovc_enc_wavelet_family wavelet_family;
		ovc_enc_wavelet_config wavelet_config;
		ovc_enc_partitioner	   partitioner;
		ovc_enc_compressor	   compressor;
		ovc_enc_entropy_coder  entropy_coder;
		ovc_enc_interleaver	   interleaver;

		bool repeat_vps;
		bool multithreading;

		ovc_enc_verbosity log_verbosity;
	} ovc_enc_config;

	typedef struct ovc_enc_nal
	{
	public:
		uint8_t* bytes;
		size_t	 size;
	} ovc_enc_nal;

	typedef struct ovc_encoder ovc_encoder;

	typedef struct ovc_enc_plane
	{
		uint8_t* data;
		size_t	 width;
		size_t	 height;
		size_t	 bit_depth;
	} ovc_enc_plane;

	typedef struct ovc_enc_picture
	{
		ovc_enc_plane		  planes[3];
		ovc_enc_chroma_format format;
		double				  framerate;
	} ovc_enc_picture;

	typedef void (*ovc_enc_logging_callback)(int, const char*, va_list);

	typedef struct ovc_encoder_api
	{
		// config
		ovc_enc_config* (*config_create)(void);
		ovc_enc_result (*config_destroy)(ovc_enc_config* in_config);
		ovc_enc_result (*config_validate)(ovc_enc_config* in_config);
		// picture
		ovc_enc_picture* (*picture_create)(ovc_encoder* in_encoder);
		ovc_enc_result (*picture_destroy)(ovc_enc_picture* in_picture);
		// encoder
		ovc_encoder* (*encoder_create)(ovc_enc_config* in_config);
		ovc_enc_result (*encoder_destroy)(ovc_encoder* in_encoder);
		ovc_enc_result (*encode)(ovc_encoder* in_encoder, ovc_enc_picture* in_picture, ovc_enc_nal** out_nals, size_t* out_num_nals);
		// misc
		void (*set_logging_callback)(ovc_enc_logging_callback in_callback);
		void (*set_logging_verbosity)(ovc_enc_verbosity in_verbosity);
	} ovc_encoder_api;

	OVC_ENC_API const ovc_encoder_api* ovc_encoder_api_get(void);

#ifdef __cplusplus
} // extern "C"
#endif
