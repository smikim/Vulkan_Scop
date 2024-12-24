#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct BITMAPFILEHEADER {
	uint16_t	bfType{0x4D42};
	uint32_t	bfSize{0};
	uint16_t	bfReserved1{0};
	uint16_t	bfReserved2{0};
	uint32_t	bfOffBits{0};
};

struct BITMAPINFOHEADER {
	uint32_t	biSize{ 0 };
	int32_t		btWidth{ 0 };
	int32_t		btHeight{ 0 };
	uint16_t	biPlanes{ 1 };
	uint16_t	biBitCount{ 0 };
	uint32_t	biCompression{ 0 };
	uint32_t	biSizeImage{ 0 };
	int32_t		biXPelsPerMeter{ 0 };
	int32_t		biYPelsPerMeter{ 0 };
	uint32_t	biClrUsed{ 0 };
	uint32_t	biClrImportant{ 0 };
};

struct RGBQUAD {
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
};

#pragma pack(pop)

struct BmpLoader
{
	BITMAPFILEHEADER			fileHeader;
	BITMAPINFOHEADER			infoHeader;
	std::unique_ptr<uint8_t[]>	data;
	std::vector<RGBQUAD> colorTable;

	void	readFile(std::string& filename);
	void	processImage(std::vector<uint8_t>& imageData);
	int		calculatePadding(int width, int bitsPerPixel);
};

