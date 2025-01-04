#include "BmpLoader.h"

void BmpLoader::readFile(std::string& filename)
{
	std::ifstream in{ filename, std::ios_base::binary};
	if (in.is_open()) {
		in.read((char*)(&fileHeader), sizeof(fileHeader));
		if (!in) {
			return ;
		}

		if (fileHeader.bfType != 0x4D42) {
			std::cerr << filename << "is not bitmap" << std::endl;
			return ;
		}

		in.read((char*)(&infoHeader), sizeof(infoHeader));
		if (!in) return ;

		if (infoHeader.biBitCount == 8) {
			uint32_t colorTableSize = infoHeader.biClrUsed ? infoHeader.biClrUsed : 256;
			colorTable.resize(colorTableSize);
			in.read(reinterpret_cast<char*>(colorTable.data()), colorTable.size() * sizeof(RGBQUAD));
			if (!in) {
				std::cerr << "Failed to read color table" << std::endl;
				return;
			}
		}

		int bytesPerPixel = infoHeader.biBitCount / 8;
		int rowSize = infoHeader.btWidth * bytesPerPixel;
		int padding = (4 - (rowSize % 4)) % 4;
		int imageSize = (rowSize + padding) * infoHeader.btHeight;

		//std::cout << infoHeader.btWidth * infoHeader.btHeight * bytesPerPixel << std::endl;
		//std::cout << imageSize << std::endl;

		in.seekg(fileHeader.bfOffBits, in.beg);
		data = std::make_unique<uint8_t[]>(imageSize);

		in.read(reinterpret_cast<char*>(data.get()), imageSize);
		if (!in) return ;

		std::cout << infoHeader.btWidth * infoHeader.btHeight * 4 << std::endl;

		
		in.close();
		
	}
	else {
		std::cerr << filename << "is not open" << std::endl;
	}

}

void BmpLoader::processImage(std::vector<uint8_t>& imageData)
{

	if (infoHeader.biBitCount == 8) {
		imageData.resize(infoHeader.btWidth * infoHeader.btHeight * 4);
	
		for (int y = 0; y < infoHeader.btHeight; ++y) {
			for (int x = 0; x < infoHeader.btWidth; ++x) {
				uint8_t paletteIndex = data[(infoHeader.btHeight - 1 - y) * infoHeader.btWidth + x];				
				//uint8_t paletteIndex = data[y * infoHeader.btWidth + x];
				
				RGBQUAD color = colorTable[paletteIndex];
				int index = (y * infoHeader.btWidth + x) * 4;
				imageData[index] = color.rgbRed;
				imageData[index + 1] = color.rgbGreen;
				imageData[index + 2] = color.rgbBlue;
				imageData[index + 3] = 255; // 알파 채널
			}
		}
	}
	else if (infoHeader.biBitCount == 24 || infoHeader.biBitCount == 32) {
		int bytesPerPixel = infoHeader.biBitCount / 8;
		int rowSize = (infoHeader.btWidth * bytesPerPixel + 3) & ~3; // 각 행의 바이트 수 (4바이트 배수로 패딩)
		imageData.resize(infoHeader.btWidth * infoHeader.btHeight * 4);
		for (int y = 0; y < infoHeader.btHeight; ++y) {
			for (int x = 0; x < infoHeader.btWidth; ++x) {
				
				//uint8_t* pixel = &data[y * rowSize + x * bytesPerPixel];
				uint8_t* pixel = &data[(infoHeader.btHeight - 1 - y) * rowSize + x * bytesPerPixel];

				int index = (y * infoHeader.btWidth + x) * 4;
				imageData[index] = pixel[2]; // Red
				imageData[index + 1] = pixel[1]; // Green
				imageData[index + 2] = pixel[0]; // Blue
				imageData[index + 3] = (bytesPerPixel == 4) ? pixel[3] : 255; // Alpha
			}
		}
	}
}

int BmpLoader::calculatePadding(int width, int bitsPerPixel) {
	int bytesPerPixel = bitsPerPixel / 8;
	int rowSize = width * bytesPerPixel;
	int padding = (4 - (rowSize % 4)) % 4;
	return padding;
}
