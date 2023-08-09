#include "image_loader.h"

#include "TImage.h"

#include <algorithm>


ImageLoader::ImageLoader(const std::string& filename): filename(filename) {}

void ImageLoader::load() {
	TImage* image = TImage::Open(filename.c_str(), TImage::EImageFileTypes::kPng);
	width = image->GetWidth();
	height = image->GetHeight();
	uint8_t* rgba_pointer = reinterpret_cast<uint8_t*>(image->GetRgbaArray());
	rgba.assign(rgba_pointer, rgba_pointer + width * height * 4);
	delete[] rgba_pointer;
	delete image;
}

uint8_t ImageLoader::getPixelR(size_t x, size_t y) const {
	return rgba[4 * (y * width + x) + 3];
}

uint8_t ImageLoader::getPixelG(size_t x, size_t y) const {
	return rgba[4 * (y * width + x) + 2];
}

uint8_t ImageLoader::getPixelB(size_t x, size_t y) const {
	return rgba[4 * (y * width + x) + 1];
}

double ImageLoader::getPixelV(size_t x, size_t y) const {
	const uint8_t* pointer = rgba.data() + 4 * (y * width + x);
	uint8_t max_component = std::max(std::max(*(pointer + 1), *(pointer + 2)), *(pointer + 3));
	return max_component / 255.0;
}

size_t ImageLoader::getWidth() const {
	return width;
}

size_t ImageLoader::getHeight() const {
	return height;
}