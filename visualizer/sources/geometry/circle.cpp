#include "circle.h"

#include <cmath>


Circle::Circle(const Point& center, double r): center(center), r(r) {}

bool Circle::isPointInside(const Point& point) const {
	return distance(center, point) <= r;
}


std::vector<Point> intersectionPoints(const Circle& first, const Circle& second) {
	std::vector<Point> result;
	double between = distance(first.center, second.center);
	double cos = (first.r / between + between / first.r -
		second.r * second.r / first.r / between) * 0.5;
	if (std::abs(cos) >= 1 + EPS)
		return result;
	Vector towards = normalized(second.center - first.center);
	if (std::abs(cos) > 1 - EPS) {
		result.push_back(first.center + sign(cos) * first.r * towards);
		return result;
	}
	double sin = std::sqrt(1 - cos * cos);
	double projection = first.r * cos;
	double perpendicular = first.r * sin;
	result.push_back(first.center + projection * towards + perpendicular * turnedLeft(towards));
	result.push_back(first.center + projection * towards + perpendicular * turnedRight(towards));
	return result;
}