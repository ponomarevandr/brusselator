#pragma once

#include "geometry/basics.h"

#include <vector>


struct Circle {
	Point center;
	double r = 0;

	Circle() = default;
	Circle(const Point& center, double r);
	bool isPointInside(const Point&) const;
};

std::vector<Point> intersectionPoints(const Circle&, const Circle&);