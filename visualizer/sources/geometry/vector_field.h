#pragma once

#include "geometry/basics.h"

#include <functional>


using CoordinateFunction = std::function<double(double, double)>;

struct VectorField {
	CoordinateFunction fx;
	CoordinateFunction fy;
	
	VectorField(CoordinateFunction fx, CoordinateFunction fy);
	Vector value(const Point&) const;
	Vector direction(const Point&) const;
};