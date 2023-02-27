#include "frame.h"


Frame::Frame(const Point& bottom_left, const Point& top_right):
	bottom_left(bottom_left), top_right(top_right) {}

Frame::Frame(double width, double height): bottom_left(0, 0), top_right(width, height) {}

Frame::Frame(const Point& bottom_left, double width, double height):
	bottom_left(bottom_left), top_right(bottom_left + Vector(width, height)) {}

const Point& Frame::getBottomLeft() const {
	return bottom_left;
}

const Point& Frame::getTopRight() const {
	return top_right;
}

double Frame::width() const {
	return top_right.x - bottom_left.x;
}

double Frame::height() const {
	return top_right.y - bottom_left.y;
}


void frameTranslate(const Frame& source, const Frame& target, Point& point) {
	point -= Vector(source.getBottomLeft());
	point.x *= target.width() / source.width();
	point.y *= target.height() / source.height();
	point += Vector(target.getBottomLeft());
}

void frameTranslate(const Frame& source, const Frame& target, std::vector<SegmentedLine>& lines) {
	for (SegmentedLine& line : lines) {
		for (Point& point : line) {
			frameTranslate(source, target, point);
		}
	}
}

void frameTranslate(const Frame& source, const Frame& target, VectorField& field) {
	auto new_fx = [source, target, fx = field.fx](double x, double y) -> double {
		Point point(x, y);
		frameTranslate(target, source, point);
		double result = fx(point.x, point.y);
		return result * target.width() / source.width();
	};
	auto new_fy = [source, target, fy = field.fy](double x, double y) -> double {
		Point point(x, y);
		frameTranslate(target, source, point);
		double result = fy(point.x, point.y);
		return result * target.height() / source.height();
	};
	field = VectorField(new_fx, new_fy);
}