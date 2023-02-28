#pragma once

#include "geometry/basics.h"
#include "geometry/frame.h"

#include <vector>


class NeighboursBase {
private:
	Frame zone;
	double max_radius;
	std::vector<std::vector<std::vector<Point>>> base;

private:
	std::pair<size_t, size_t> getSector(const Point&) const;

public:
	NeighboursBase(const Frame& zone, double max_radius);
	bool addPoint(const Point&);
	bool hasNeighbours(const Point& point, double radius) const;
	std::vector<Point> getNeighbours(const Point& point, double radius) const;
};