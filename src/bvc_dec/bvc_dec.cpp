#include "bvc_dec.h"

bvc_decoder::bvc_decoder()
{
}

bvc_dec_result bvc_decoder::init()
{
	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::decode_nal(bvc_dec_nal* in_nal_unit)
{
	decoded_picture = new bvc_decoded_picture();

	decoded_picture->bytes = in_nal_unit->bytes;
	decoded_picture->size = in_nal_unit->size;

	return bvc_dec_result::BVC_DEC_OK;
}

bvc_dec_result bvc_decoder::get_picture(bvc_decoded_picture* out_picture)
{
	out_picture = decoded_picture;

	return bvc_dec_result::BVC_DEC_OK;
}