#pragma once

typedef enum
{
	BVC_SPIHT_SKIP,
	BVC_SPIHT_ENABLE,
} bvc_spiht;

typedef enum
{
	BVC_SPIHT_TYPE_A,
	BVC_SPIHT_TYPE_B
} bvc_spiht_type;

class bvc_spiht_set
{
public:
	bvc_spiht_set(size_t in_x, size_t in_y, bvc_spiht_type in_type);

public:
	bvc_spiht_type type;
	size_t		   x;
	size_t		   y;
};

class bvc_spiht_pixel
{
public:
	bvc_spiht_pixel(size_t in_x, size_t in_y);

public:
	size_t x;
	size_t y;
};