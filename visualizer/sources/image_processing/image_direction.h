#pragma once

#include "geometry/basics.h"
#include "image_processing/image_loader.h"

#include <vector>


class ImageDirection {
private:
	struct Offset {
		int dx;
		int dy;
		size_t r_squared;

		Offset(int dx, int dy);
		bool operator<(const Offset&) const;
		double r() const;
		double polarCos() const;
		double polarSin() const;
	};

private:
	static const double PI;
	static const size_t MAX_R_SQUARED;
	static const double MIN_DIRECTION_LENGTH;
	ImageLoader& image;
	std::vector<std::vector<Vector>> directions;
	std::vector<Offset> offsets;

private:
	bool areCoordinatesValid(int x, int y) const;
	void prepareOffsets();
	void calculateDirection(int center_x, int center_y);

public:
	explicit ImageDirection(ImageLoader& image);
	void calculate();
	Vector getDirection(size_t x, size_t y) const;
};