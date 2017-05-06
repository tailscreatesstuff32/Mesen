#pragma once
#include "stdafx.h"

class PNGHelper
{
private:
	static int DecodePNG(vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

public:
	static bool WritePNG(std::stringstream &stream, uint8_t* buffer, uint32_t xSize, uint32_t ySize, uint32_t bitsPerPixel = 32);
	static bool WritePNG(string filename, uint8_t* buffer, uint32_t xSize, uint32_t ySize, uint32_t bitsPerPixel = 32);
	static void ReadPNG(string filename, vector<uint8_t> &pngData, uint32_t &pngWidth, uint32_t &pngHeight);
};