#pragma once

#include <vector>
#include <string>


class ImageLoader {
private:
	std::string filename;
	std::vector<uint8_t> rgba;
	size_t width;
	size_t height;

public:
	explicit ImageLoader(const std::string& filename);
	void load();
	uint8_t getPixelR(size_t x, size_t y) const;
	uint8_t getPixelG(size_t x, size_t y) const;
	uint8_t getPixelB(size_t x, size_t y) const;
	double getPixelV(size_t x, size_t y) const;
	size_t getWidth() const;
	size_t getHeight() const;
};