#include "vector_field.h"


VectorField::VectorField(CoordinateFunction fx, CoordinateFunction fy): fx(fx), fy(fy) {}

Vector VectorField::value(const Point& point) const {
	return Vector(fx(point.x, point.y), fy(point.x, point.y));
}

Vector VectorField::direction(const Point& point) const {
	return normalized(value(point));
}