#pragma once

#include "config.h"
#include "result.h"
#include "nal.h"

class BvcEncoder
{
public:
	BvcEncoder();

	BvcEncResult Init(BvcEncConfig* InConfig);
	BvcEncResult Encode(const uint8_t* InPictureBytes, BvcEncNal** OutNalUnits, int* OutNumNalUnits);

private:
	int GetFormatSizeInBytes(BvcChromaFormat InFormat);

private:
	BvcEncConfig Config;
};