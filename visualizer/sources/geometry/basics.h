#pragma once

#include <vector>


struct Point;
struct Vector;


struct Point {
	double x;
	double y;

	Point(double x, double y);
	explicit Point(const Vector&);
};


struct Vector {
	double x;
	double y;

	Vector(double x, double y);
	explicit Vector(const Point&);
};


class SegmentedLine {
private:
	std::vector<double> xs;
	std::vector<double> ys;

public:
	SegmentedLine() = default;
	void push_back(const Point&);
	Point operator[](size_t index) const;
	size_t size() const;
	const double* xsBuffer() const;
	const double* ysBuffer() const;
};