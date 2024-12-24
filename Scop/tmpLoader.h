#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#pragma pack(push, 1)
struct BMPFileHeader {
	uint16_t file_type{ 0x4D42 };
	uint32_t file_size{ 0 };
	uint16_t reserved1{ 0 };
	uint16_t reserved2{ 0 };
	uint32_t offset{ 0 };
};

struct BMPInfoHeader {
	uint32_t size{ 0 };
	int32_t width{ 0 };
	int32_t height{ 0 };
	uint16_t planes{ 1 };
	uint16_t bit_count{ 0 };
	uint32_t compression{ 0 };
	uint32_t size_image{ 0 };
	int32_t x_pixels_per_meter{ 0 };
	int32_t y_pixels_per_meter{ 0 };
	uint32_t colors_used{ 0 };
	uint32_t colors_important{ 0 };
};

struct BMPColorHeader {
	uint32_t red_mask{ 0x00ff0000 };
	uint32_t green_mask{ 0x0000ff00 };
	uint32_t blue_mask{ 0x000000ff };
	uint32_t alpha_mask{ 0xff000000 };
	uint32_t color_space_type{ 0x73524742 };
};
#pragma pack(pop)

struct BMP {
	BMPFileHeader file_header;
	BMPInfoHeader info_header;
	BMPColorHeader color_header;
	std::vector<uint8_t> data;

	BMP(const char* filepath) { read(filepath); }

	void read(const char* filepath) {
		std::ifstream input{ filepath, std::ios_base::binary };
		if (input) {
			input.read((char*)&file_header, sizeof(file_header));
			if (file_header.file_type != 0x4D42) {
				throw std::runtime_error("Error! Unrecognized file format.");
			}

			input.read((char*)&info_header, sizeof(info_header));

			if (info_header.bit_count == 32) {
				if (info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
					input.read((char*)&color_header, sizeof(color_header));
					check_color_header(color_header);
				}
				else {
					std::cerr << "Warning! The file \"" << filepath << "\" does not seem to contain a color header!\n";
					throw std::runtime_error("Error! Unrecognized file format.");
				}
			}
			else {
				throw std::runtime_error("Error! Only 32-bit images are supported.");
			}

			// Jump to the pixel data location
			input.seekg(file_header.offset, input.beg);
			data.resize(info_header.width * info_header.height * info_header.bit_count / 8);

			input.read((char*)data.data(), data.size());
			if (info_header.bit_count == 32) {
				for (size_t i = 0; i < data.size(); i += 4) {
					std::swap(data[i], data[i + 2]); // Swap to RGBA
				}
			}

			// Check if the row padding is multiple of 4
			if (info_header.width % 4 == 0) {
				input.read((char*)data.data(), data.size());
				file_header.file_size += static_cast<uint32_t>(data.size());
			}
			else {
				row_stride = info_header.width * info_header.bit_count / 8;
				uint32_t new_stride = make_stride_aligned(4);
				std::vector<uint8_t> padding_row(new_stride - row_stride);

				for (int y = 0; y < info_header.height; ++y) {
					input.read((char*)(data.data() + row_stride * y), row_stride);
					input.read((char*)padding_row.data(), padding_row.size());
				}
				file_header.file_size +=
					static_cast<uint32_t>(data.size()) +
					info_header.height * static_cast<uint32_t>(padding_row.size());
			}
		}
		else {
			throw std::runtime_error("Unable to open the input image file.");
		}
	}

private:
	uint32_t row_stride{ 0 };

	// Add 1 to the row_stride until it is divisible with align_stride
	uint32_t make_stride_aligned(uint32_t align_stride) {
		uint32_t new_stride = row_stride;
		while (new_stride % align_stride != 0) {
			new_stride++;
		}
		return new_stride;
	}

	// Check if the pixel data is stored as BGRA and if the color space type is sRGB
	void check_color_header(BMPColorHeader& bmp_color_header) {
		BMPColorHeader expected_color_header;
		if (expected_color_header.red_mask != bmp_color_header.red_mask ||
			expected_color_header.blue_mask != bmp_color_header.blue_mask ||
			expected_color_header.green_mask != bmp_color_header.green_mask ||
			expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
			throw std::runtime_error("Unexpected color mask format! The program expects the pixel "
				"data to be in the BGRA format");
		}
		if (expected_color_header.color_space_type != bmp_color_header.color_space_type) {
			throw std::runtime_error(
				"Unexpected color space type! The program expects sRGB values");
		}
	}
};
