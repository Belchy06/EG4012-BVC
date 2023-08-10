#include "bvc_dec.h"

BvcDecoder::BvcDecoder()
{
}

BvcDecResult BvcDecoder::Init()
{
	return BvcDecResult::BVC_DEC_OK;
}

BvcDecResult BvcDecoder::DecodeNal(BvcDecNal* InNalUnit)
{
	DecodedPicture = new BvcDecodedPicture();

	DecodedPicture->Bytes = InNalUnit->Bytes;
	DecodedPicture->Size = InNalUnit->Size;

	return BvcDecResult::BVC_DEC_OK;
}

BvcDecResult BvcDecoder::GetPicture(BvcDecodedPicture& OutPicture)
{
	OutPicture = *DecodedPicture;

	return BvcDecResult::BVC_DEC_OK;
}