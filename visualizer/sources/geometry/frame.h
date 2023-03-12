#pragma once

#include "geometry/basics.h"
#include "geometry/vector_field.h"

#include <vector>


class Frame {
private:
	Point bottom_left;
	Point top_right;

public:
	Frame() = default;
	Frame(const Point& bottom_left, const Point& top_right);
	Frame(double width, double height);
	Frame(const Point& bottom_left, double width, double height);
	const Point& getBottomLeft() const;
	const Point& getTopRight() const;
	double width() const;
	double height() const;
	bool isPointInside(const Point&) const;
};

extern const Frame BASIC_FRAME;

void frameTranslate(const Frame& source, const Frame& target, Point& point);
void frameTranslate(const Frame& source, const Frame& target, std::vector<SegmentedLine>& lines);
void frameTranslate(const Frame& source, const Frame& target, VectorField& field);