#include "basics.h"


Point::Point(double x, double y): x(x), y(y) {}

Point::Point(const Vector& vector): x(vector.x), y(vector.y) {}


Vector::Vector(double x, double y): x(x), y(y) {}

Vector::Vector(const Point& point): x(point.x), y(point.y) {}


void SegmentedLine::push_back(const Point& point) {
	xs.push_back(point.x);
	ys.push_back(point.y);
}

Point SegmentedLine::operator[](size_t index) const {
	return Point(xs[index], ys[index]);
}

size_t SegmentedLine::size() const {
	return xs.size();
}

const double* SegmentedLine::xsBuffer() const {
	return xs.data();
}

const double* SegmentedLine::ysBuffer() const {
	return ys.data();
}