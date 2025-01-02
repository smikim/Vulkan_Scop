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

		/*if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32)
		{
			std::cerr << "bitCount is not 24 or 32" << std::endl;
			return nullptr;
		}*/

		//int bytesPerPixel = infoHeader.biBitCount / 8;
		//int padding = calculatePadding(infoHeader.btWidth, bytesPerPixel);

		//int imageSize = infoHeader.btWidth * infoHeader.btHeight * bytesPerPixel;

		int bytesPerPixel = infoHeader.biBitCount / 8;
		int rowSize = infoHeader.btWidth * bytesPerPixel;
		int padding = (4 - (rowSize % 4)) % 4;
		int imageSize = (rowSize + padding) * infoHeader.btHeight;

		std::cout << infoHeader.btWidth * infoHeader.btHeight * bytesPerPixel << std::endl;
		std::cout << imageSize << std::endl;

		//std::unique_ptr<uint8_t[]> readData = std::make_unique<uint8_t[]>(infoHeader.btWidth * infoHeader.btHeight * bytesPerPixel);
		
		//std::unique_ptr<uint8_t[]> readData = std::make_unique<uint8_t[]>(imageSize);

		in.seekg(fileHeader.bfOffBits, in.beg);
		data = std::make_unique<uint8_t[]>(imageSize);

		//in.read(reinterpret_cast<char*>(readData.get()), imageSize);
		in.read(reinterpret_cast<char*>(data.get()), imageSize);
		if (!in) return ;

		//std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(infoHeader.btWidth * infoHeader.btHeight * 4);

		std::cout << infoHeader.btWidth * infoHeader.btHeight * 4 << std::endl;

		//int bytesPerPixel = infoHeader.biBitCount / 8;
		//int padding = calculatePadding(infoHeader.btWidth, bytesPerPixel);


		//int g = 0;
		//int k = 0;
		//for (int i = 0; i < infoHeader.btHeight; i++)
		//{
		//	for (int j = 0; j < infoHeader.btWidth; j++) {
		//		k = (infoHeader.btHeight - i - 1) * (infoHeader.btWidth * bytesPerPixel + padding) + j * bytesPerPixel;
		//		g = i * infoHeader.btWidth + j;

		//		//int k = i * (infoHeader.btWidth * bytesPerPixel + padding) + j * bytesPerPixel;
		//		//int g = (infoHeader.btHeight - i - 1) * infoHeader.btWidth + j;

		//		//int k = i * (infoHeader.btWidth * bytesPerPixel + padding) + j * bytesPerPixel;
		//		//int g = (infoHeader.btHeight - i - 1) * infoHeader.btWidth + (infoHeader.btWidth - j - 1);


		//		if (infoHeader.biBitCount == 8) {
		//			// 그레이스케일 이미지의 경우
		//			buffer[g * 4] = readData[k + 2];
		//			buffer[g * 4 + 1] = readData[k + 1];
		//			buffer[g * 4 + 2] = readData[k];
		//			buffer[g * 4 + 3] = 255; // 알파 채널을 255로 설정
		//			//buffer[g * 4 + 3] = readData[k]; // 알파 채널을 255로 설정

		//		}
		//		else if (infoHeader.biBitCount == 24) {
		//			// 24비트 이미지의 경우
		//			buffer[g * 4] = readData[k + 2];
		//			buffer[g * 4 + 1] = readData[k + 1];
		//			buffer[g * 4 + 2] = readData[k];
		//			buffer[g * 4 + 3] = 255; // 알파 채널을 255로 설정
		//		}
		//		else if (infoHeader.biBitCount == 32) {
		//			// 32비트 이미지의 경우
		//			buffer[g * 4] = readData[k + 2];
		//			buffer[g * 4 + 1] = readData[k + 1];
		//			buffer[g * 4 + 2] = readData[k];
		//			buffer[g * 4 + 3] = readData[k + 3];
		//		}

		//		/*buffer[g * 4] = data[k + 2];
		//		buffer[g * 4 + 1] = data[k + 1];
		//		buffer[g * 4 + 2] = data[k];
		//		buffer[g * 4 + 3] = (infoHeader.biBitCount == 24) ? 255 : data[k + 3];*/

		//	}
		//}
		//std::cout << "g : " <<  g << std::endl;
		//std::cout << "k : " <<  k << std::endl;

		//data = std::move(buffer);
		
		in.close();
		
	}
	else {
		std::cerr << filename << "is not open" << std::endl;
	}

}

void BmpLoader::processImage(std::vector<uint8_t>& imageData)
{
	//std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(infoHeader.btWidth * infoHeader.btHeight * 4);

	if (infoHeader.biBitCount == 8) {
		imageData.resize(infoHeader.btWidth * infoHeader.btHeight * 4);
		// 8비트 이미지의 경우 팔레트를 사용하여 실제 RGB 값을 얻습니다.
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
		//imageData.resize(infoHeader.btWidth * infoHeader.btHeight * 4);
		// 24비트 및 32비트 이미지의 경우 직접 RGB 값을 읽습니다.
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

		//// 24비트 및 32비트 이미지의 경우 직접 RGB 값을 읽습니다.
		//int bytesPerPixel = infoHeader.biBitCount / 8;
		//for (int y = 0; y < infoHeader.btHeight; ++y) {
		//	for (int x = 0; x < infoHeader.btWidth; ++x) {
		//		uint8_t* pixel = &data[(y * infoHeader.btWidth + x) * bytesPerPixel];
		//		uint8_t blue = pixel[0];
		//		uint8_t green = pixel[1];
		//		uint8_t red = pixel[2];
		//		// 32비트 이미지의 경우 알파 채널도 있습니다.
		//		uint8_t alpha = (bytesPerPixel == 4) ? pixel[3] : 255;
		//		std::cout << "Pixel (" << x << ", " << y << "): "
		//			<< "R=" << static_cast<int>(red) << " "
		//			<< "G=" << static_cast<int>(green) << " "
		//			<< "B=" << static_cast<int>(blue) << " "
		//			<< "A=" << static_cast<int>(alpha) << std::endl;
		//	}
		//}
	}
}

int BmpLoader::calculatePadding(int width, int bitsPerPixel) {
	int bytesPerPixel = bitsPerPixel / 8;
	int rowSize = width * bytesPerPixel;
	int padding = (4 - (rowSize % 4)) % 4;
	return padding;
}
