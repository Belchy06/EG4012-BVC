#include <fstream>
#include <iostream>
#include <string>

#include "ppm.h"

int ppm::read(std::string in_source_path, ovc_picture* out_picture)
{
	std::ifstream infile(in_source_path, std::ifstream::binary);
	// Examine if the file could be opened successfully
	if (!infile.is_open())
	{
		std::cout << "Failed to open " << in_source_path << std::endl;
		return -1;
	}

	std::string magic;
	infile >> magic;
	infile.seekg(1, infile.cur);
	char c;
	infile.get(c);
	if (c == '#')
	{
		// We got comments in the PPM image and skip the comments
		while (c != '\n')
		{
			infile.get(c);
		}
	}
	else
	{
		infile.seekg(-1, infile.cur);
	}

	int width, height, max;
	infile >> width >> height >> max;
	if (max != 255)
	{
		std::cout << "Failed to read " << in_source_path << std::endl;
		std::cout << "Got PPM maximum value: " << max << std::endl;
		std::cout << "Maximum pixel has to be 255" << std::endl;
	}

	uint8_t* buffer = new uint8_t[width * height * 3];
	uint8_t* y_buf = new uint8_t[width * height];

	if (magic == "P6")
	{
		// Move curser once to skip '\n'
		infile.seekg(1, infile.cur);
		infile.read(reinterpret_cast<char*>(buffer), width * height * 3);

		// convert rgb input to yuv
		for (int i = 0; i < width * height; i)
		{
			uint8_t r = (uint8_t)buffer[i + 0];
			uint8_t g = (uint8_t)buffer[i + 1];
			uint8_t b = (uint8_t)buffer[i + 2];

			uint8_t y = (uint8_t)(r * .299000 + g * .587000 + b * .114000);
			uint8_t u = (uint8_t)(r * -.168736 + g * -.331264 + b * .500000 + 128);
			uint8_t v = (uint8_t)(r * .500000 + g * -.418688 + b * -.081312 + 128);

			y_buf[i] = y;
			buffer[i + 0] = y;
			buffer[i + 1] = u;
			buffer[i + 2] = v;

			i += 3;
		}
	}
	else
	{
		std::cout << "Unrecognized PPM format" << std::endl;
		std::cout << "Got PPM magic number: " << magic << std::endl;
		std::cout << "PPM magic number should be either P3 or P6" << std::endl;
		return -1;
	}
	out_picture->Y = new uint8_t[width * height]{ 0 };
	memcpy(out_picture->Y, y_buf, width * height);
	out_picture->info.height = height;
	out_picture->info.width = width;

	return 0;
}

int ppm::write(std::string in_output_path, ovc_picture* in_picture, std::string in_magic)
{
	std::ofstream outfile(in_output_path, std::ofstream::binary);
	if (outfile.fail())
	{
		std::cout << "Failed to write " << in_output_path << std::endl;
		return 1;
	}

	outfile << in_magic << "\n"
			<< in_picture->info.width << " " << in_picture->info.height << "\n"
			<< 255 << "\n";

	if (in_magic == "P6")
	{
		for (size_t j = 0; j < in_picture->info.height; ++j)
		{
			for (size_t i = 0; i < in_picture->info.width; ++i)
			{
				uint8_t Y = in_picture->Y[i + j * in_picture->info.width];
				uint8_t R = (uint8_t)(Y);
				uint8_t G = (uint8_t)(Y);
				uint8_t B = (uint8_t)(Y);
				outfile << static_cast<char>(R)
						<< static_cast<char>(G)
						<< static_cast<char>(B);
			}
		}
	}
	else
	{
		std::cout << "Unrecognized PPM format" << std::endl;
		std::cout << "Got PPM magic number: " << in_magic << std::endl;
		std::cout << "PPM magic number should be either P3 or P6" << std::endl;
		return 1;
	}

	return 0;
}