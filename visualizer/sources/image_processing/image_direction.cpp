#include "image_direction.h"

#include <cmath>
#include <algorithm>

#include <iostream>


ImageDirection::Offset::Offset(int dx, int dy): dx(dx), dy(dy), r_squared(dx * dx + dy * dy) {}

bool ImageDirection::Offset::operator<(const Offset& other) const {
	if (r_squared != other.r_squared)
		return r_squared < other.r_squared;
	if (dx != other.dx)
		return dx < other.dx;
	return dy < other.dy;
}

double ImageDirection::Offset::r() const {
	return std::sqrt(r_squared);
}

double ImageDirection::Offset::polarCos() const {
	return dx / r();
}

double ImageDirection::Offset::polarSin() const {
	return dy / r();
}


const double ImageDirection::PI = 3.1415926535898;
const size_t ImageDirection::MAX_R_SQUARED = 10; // 25
const double ImageDirection::MIN_DIRECTION_LENGTH = 0.15;


bool ImageDirection::areCoordinatesValid(int x, int y) const {
	return 0 <= x && 0 <= y && x < image.getWidth() && y < image.getHeight();
}

void ImageDirection::prepareOffsets() {
	offsets.clear();
	int max_offset = 0;
	while ((max_offset + 1) * (max_offset + 1) <= MAX_R_SQUARED) {
		++max_offset;
	}
	for (int i = -max_offset; i <= max_offset; ++i) {
		for (int j = -max_offset; j <= max_offset; ++j) {
			offsets.emplace_back(i, j);
		}
	}
	std::sort(offsets.begin(), offsets.end());
	offsets.erase(offsets.begin());
	while (!offsets.empty() && offsets.back().r_squared > MAX_R_SQUARED) {
		offsets.pop_back();
	}
}

void ImageDirection::calculateDirection(int center_x, int center_y) {
	double mean = 0;
	size_t valid_pixels = 0;
	for (const Offset& offset : offsets) {
		int x = center_x + offset.dx;
		int y = center_y + offset.dy;
		if (!areCoordinatesValid(x, y))
			continue;
		mean += image.getPixelV(x, y);
		++valid_pixels;
	}
	if (valid_pixels == 0) {
		directions[center_x][center_y] = Vector(0, 0);
		return;
	}
	mean /= valid_pixels;
	double x_coef = 0;
	double y_coef = 0;
	for (const Offset& offset : offsets) {
		int x = center_x + offset.dx;
		int y = center_y + offset.dy;
		double value = areCoordinatesValid(x, y) ? image.getPixelV(x, y) : mean;
		x_coef += value * offset.polarCos() / offset.r();
		y_coef += value * offset.polarSin() / offset.r();
	}
	directions[center_x][center_y] = Vector(-y_coef, x_coef);
	if (directions[center_x][center_y].length() < MIN_DIRECTION_LENGTH)
		directions[center_x][center_y] = Vector(0, 0);
}

ImageDirection::ImageDirection(ImageLoader& image): image(image) {}

void ImageDirection::calculate() {
	prepareOffsets();
	directions.resize(image.getWidth());
	for (size_t i = 0; i < image.getWidth(); ++i) {
		directions[i].resize(image.getHeight());
		for (size_t j = 0; j < image.getHeight(); ++j) {
			calculateDirection(i, j);
		}
	}
}

Vector ImageDirection::getDirection(size_t x, size_t y) const {
	return directions[x][y];
}