#pragma once

#include "picture.h"
#include "result.h"
#include "nal.h"

class BvcDecoder
{
public:
	BvcDecoder();

	BvcDecResult Init();
	BvcDecResult DecodeNal(BvcDecNal* InNalUnit);
	BvcDecResult GetPicture(BvcDecodedPicture& OutPicture);

private:
	BvcDecodedPicture* DecodedPicture;
};