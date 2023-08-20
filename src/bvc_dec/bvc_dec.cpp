#include "bvc_dec.h"

#include <cmath>
#include <cstring>

bvc_decoder::bvc_decoder()
	: picture_ready(false)
{
}

bvc_dec_result bvc_decoder::init()
{
	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::decode_nal(const bvc_nal* in_nal_unit)
{
	uint8_t* nal_bytes = new uint8_t[in_nal_unit->size]{ 0 };
	memcpy(nal_bytes, in_nal_unit->bytes, in_nal_unit->size);

	/*
	 +---------------+---------------+---------------+---------------+
	 |0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
	 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 | W F |   W C   | P | E |S|     |           NUM_LEVELS          |
	 +---------------+---------------+---------------+---------------+
	 |          NUM_STREAMS          |           STREAM_ID           |
	 +---------------+---------------+---------------+---------------+
	 |                             WIDTH                             |
	 +---------------+---------------+---------------+---------------+
	 |                             HEIGHT                            |
	 +---------------+---------------+---------------+---------------+
	 |                             NUM_SYM                           |
	 +---------------+---------------+---------------+---------------+
	 |                             BPP                               |
	 +---------------+---------------+---------------+---------------+
	 |                             STEP                              |
	 +---------------+---------------+---------------+---------------+
	*/

	// Parse header for configuration
	size_t	byte_idx = 0;
	uint8_t config = 0;
	config = nal_bytes[byte_idx++];
	bvc_wavelet_family wavelet_family = (bvc_wavelet_family)((config >> 5) & 0b111);
	bvc_wavelet_config wavlet_config = { .value = (uint8_t)((config >> 0) & 0b11111) };
	config = nal_bytes[byte_idx++];
	bvc_partition	  partition_type = (bvc_partition)((config >> 6) & 0b11);
	bvc_entropy_coder entropy_coder = (bvc_entropy_coder)((config >> 4) & 0b11);
	bvc_spiht		  spiht = (bvc_spiht)((config >> 3) & 0b1);

	// Intialise components based on specified config
	wavelet_recomposer = bvc_wavelet_recomposer_factory::create_wavelet_recomposer(wavelet_family, wavlet_config);
	spiht_decoder = bvc_spiht_decoder_factory::create_spiht_decoder(spiht);
	departitioner = bvc_departitioner_factory::create_departitioner(partition_type);
	entropy_decoder = bvc_entropy_decoder_factory::create_entropy_decoder(entropy_coder);

	uint16_t num_levels = 0;
	num_levels |= nal_bytes[byte_idx++] << 8;
	num_levels |= nal_bytes[byte_idx++] << 0;

	uint16_t num_streams = 0;
	num_streams |= nal_bytes[byte_idx++] << 8;
	num_streams |= nal_bytes[byte_idx++] << 0;

	uint16_t stream_id = 0;
	stream_id |= nal_bytes[byte_idx++] << 8;
	stream_id |= nal_bytes[byte_idx++] << 0;

	// Extract picture settings from header
	size_t width = 0;
	width |= nal_bytes[byte_idx++] << 24;
	width |= nal_bytes[byte_idx++] << 16;
	width |= nal_bytes[byte_idx++] << 8;
	width |= nal_bytes[byte_idx++] << 0;

	size_t height = 0;
	height |= nal_bytes[byte_idx++] << 24;
	height |= nal_bytes[byte_idx++] << 16;
	height |= nal_bytes[byte_idx++] << 8;
	height |= nal_bytes[byte_idx++] << 0;

	size_t num_bytes = 0;
	num_bytes |= nal_bytes[byte_idx++] << 24;
	num_bytes |= nal_bytes[byte_idx++] << 16;
	num_bytes |= nal_bytes[byte_idx++] << 8;
	num_bytes |= nal_bytes[byte_idx++] << 0;

	float bpp = 0;
	union
	{
		int	  bpp_int = 0;
		float bpp_float;
	} bits_per_pixel;

	bits_per_pixel.bpp_int |= nal_bytes[byte_idx++] << 24;
	bits_per_pixel.bpp_int |= nal_bytes[byte_idx++] << 16;
	bits_per_pixel.bpp_int |= nal_bytes[byte_idx++] << 8;
	bits_per_pixel.bpp_int |= nal_bytes[byte_idx++] << 0;
	bpp = bits_per_pixel.bpp_float;

	int step = 0;
	step |= nal_bytes[byte_idx++] << 24;
	step |= nal_bytes[byte_idx++] << 16;
	step |= nal_bytes[byte_idx++] << 8;
	step |= nal_bytes[byte_idx++] << 0;

	// Decode
	//                                                num_bytes              num_symbols (bits)
	entropy_decoder->decode(nal_bytes + byte_idx, in_nal_unit->size - byte_idx, num_bytes << 3);
	uint8_t* entropy_decoded_bytes = new uint8_t();
	size_t	 entropy_decoded_size = 0;
	entropy_decoder->flush(&entropy_decoded_bytes, &entropy_decoded_size);

	spiht_decoder->decode(entropy_decoded_bytes, entropy_decoded_size, width, height, { .bpp = bpp, .num_levels = num_levels, .step = step });
	matrix<double> partition = matrix<double>(height, width);
	spiht_decoder->flush(partition);

	// TODO (belchy06): Flow should be something like
	/*
		if(partitions.contains(stream_id))
		{
			// we've already seen this partition
			// this means we've lost some nals. decode what we have
			decode(partitions)

			partitions.clear()
			partitions.insert(stream_id, matrix)
		}
		else
		{
			partitions.insert(stream_id, matrix)

			if(partitions.size() == num_streams)
			{
				decode(partitions)
			}
		}
	*/
	partitions[stream_id] = partition;

	if (partitions.size() == num_streams)
	{
		std::vector<matrix<double>> parts;
		for (size_t i = 0; i < num_streams; i++)
		{
			parts.push_back(partitions[i]);
		}
		matrix<double> full = departitioner->departition(parts, num_levels);

		bvc_wavelet_decomposition_2d<double> decomposition = bvc_wavelet_decomposition_2d<double>::from_matrix(full, num_levels);
		matrix<double>						 image = wavelet_recomposer->recompose(decomposition, full.size());

		std::vector<uint8_t> Y;
		for (size_t y = 0; y < image.get_num_rows(); y++)
		{
			for (size_t x = 0; x < image.get_num_columns(); x++)
			{
				double pixel = trunc(image(y, x));
				Y.push_back((uint8_t)pixel);
			}
		}

		picture.info.height = image.get_num_rows();
		picture.info.width = image.get_num_columns();
		picture.info.bit_depth = 8;
		picture.Y = new uint8_t[image.get_num_rows() * image.get_num_columns()]{ 0 };
		memcpy(picture.Y, Y.data(), Y.size());

		picture_ready = true;
	}

	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::get_picture(bvc_picture* out_picture)
{
	if (!picture_ready)
	{
		// We haven't received enough NALs to produce a full picture
		return bvc_dec_result::BVC_DEC_NO_PICTURE;
	}

	// TODO (belchy06): Parse headers
	out_picture->Y = new uint8_t[picture.info.width * picture.info.height * (picture.info.bit_depth >> 3)]{ 0 };
	out_picture->U = nullptr;
	out_picture->V = nullptr;
	memcpy(out_picture->Y, picture.Y, (picture.info.width * picture.info.height * (picture.info.bit_depth >> 3)));
	out_picture->info.framerate = 29.97f;
	out_picture->info.width = picture.info.width;
	out_picture->info.height = picture.info.height;
	out_picture->info.bit_depth = 8;
	out_picture->info.format = bvc_chroma_format::BVC_CHROMA_FORMAT_MONOCHROME;

	return bvc_dec_result::BVC_DEC_OK;
}

int bvc_decoder::get_size_in_bytes(bvc_chroma_format in_format)
{
	int picture_samples = 0;
	if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_MONOCHROME)
	{
		picture_samples = 176 * 144;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_420)
	{
		picture_samples = (3 * (176 * 144)) >> 1;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_422)
	{
		picture_samples = 2 * 176 * 144;
	}
	else if (in_format == bvc_chroma_format::BVC_CHROMA_FORMAT_444)
	{
		picture_samples = 3 * 176 * 144;
	}
	return picture_samples;
}