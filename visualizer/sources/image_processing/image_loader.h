#pragma once

#include "TImage.h"

#include <vector>
#include <string>
#include <memory>


class ImageLoader {
private:
	std::string filename;
	std::unique_ptr<TImage> image;
	std::vector<uint8_t> rgba;

public:
	explicit ImageLoader(const std::string& filename);
	void load();
	uint8_t getPixelR(size_t x, size_t y) const;
	uint8_t getPixelG(size_t x, size_t y) const;
	uint8_t getPixelB(size_t x, size_t y) const;
	double getPixelV(size_t x, size_t y) const;
};