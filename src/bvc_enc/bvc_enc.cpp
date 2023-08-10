#include "bvc_enc.h"

BvcEncoder::BvcEncoder()
{
}

BvcEncResult BvcEncoder::Init(BvcEncConfig* InConfig)
{
	// Validate config
	if (InConfig->Width < 0)
	{
		return BvcEncResult::BVC_ENC_INVALID_DIMENSIONS;
	}

	if (InConfig->Height < 0)
	{
		return BvcEncResult::BVC_ENC_INVALID_DIMENSIONS;
	}

	if (InConfig->Format == BvcChromaFormat::BVC_ENC_CHROMA_FORMAT_UNDEFINED)
	{
		return BvcEncResult::BVC_ENC_INVALID_FORMAT;
	}

	Config = InConfig;
	return BvcEncResult::BVC_ENC_OK;
}

BvcEncResult BvcEncoder::Encode(const uint8_t* InPictureBytes, BvcEncNal** OutNalUnits, int* OutNumNalUnits)
{
	// TODO (belchy06): Actually compress lmao
	std::vector<BvcEncNal> OutputNals;
	BvcEncNal			   Nal;
	Nal.Bytes = InPictureBytes;
	Nal.Size = Config.Width * Config.Height * GetFormatSizeInBytes(Config.Format);

	OutputNals.push_back(Nal);
	*OutNumNalUnits = static_cast<int>(OutputNals.size());
	*OutNalUnits = &OutputNals[0];

	return BvcEncResult::BVC_ENC_OK;
}

int BvcEncoder::GetFormatSizeInBytes(BvcChromaFormat InFormat)
{
	switch (InFormat)
	{
		case BvcChromaFormat::BVC_ENC_CHROMA_FORMAT_MONOCHROME:
			return 1;
		case BvcChromaFormat::BVC_ENC_CHROMA_FORMAT_420:
			// TODO (belchy06): Fix
			return 3;
		case BvcChromaFormat::BVC_ENC_CHROMA_FORMAT_422:
			// TODO (belchy06): Fix
			return 3;
		case BvcChromaFormat::BVC_ENC_CHROMA_FORMAT_444:
			return 3;
		case BvcChromaFormat::BVC_ENC_CHROMA_FORMAT_UNDEFINED:
		default:
			return 0;
	}
}