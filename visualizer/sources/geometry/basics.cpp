#include "basics.h"

#include <iostream>
#include <cmath>


const double PI = 3.14159265358979323846;


Point::Point(double x, double y): x(x), y(y) {}

Point::Point(const Vector& vector): x(vector.x), y(vector.y) {}

Point& Point::operator+=(const Vector& vector) {
	x += vector.x;
	y += vector.y;
	return *this;
}

Point& Point::operator-=(const Vector& vector) {
	x -= vector.x;
	y -= vector.y;
	return *this;
}

void Point::print() const {
	std::cout << "Point(" << x << ", " << y << ")\n";
}


Vector::Vector(double x, double y): x(x), y(y) {}

Vector::Vector(const Point& point): x(point.x), y(point.y) {}

Vector& Vector::operator+=(const Vector& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector& Vector::operator-=(const Vector& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector& Vector::operator*=(double coefficient) {
	x *= coefficient;
	y *= coefficient;
	return *this;
}

Vector& Vector::operator/=(double coefficient) {
	x /= coefficient;
	y /= coefficient;
	return *this;
}

double Vector::lengthSquared() const {
	return x * x + y * y;
}

double Vector::length() const {
	return std::sqrt(lengthSquared());
}

void Vector::print() const {
	std::cout << "Vector(" << x << ", " << y << ")\n";
}


Point operator+(const Point& point, const Vector& vector) {
	Point result = point;
	result += vector;
	return result;
}

Point operator-(const Point& point, const Vector& vector) {
	Point result = point;
	result -= vector;
	return result;
}

Vector operator-(const Point& end, const Point& begin) {
	return Vector(end.x - begin.x, end.y - begin.y);
}

Vector operator+(const Vector& first, const Vector& second) {
	Vector result = first;
	result += second;
	return result;
}

Vector operator-(const Vector& first, const Vector& second) {
	Vector result = first;
	result -= second;
	return result;
}

Vector operator*(double coefficient, const Vector& vector) {
	Vector result = vector;
	result *= coefficient;
	return result;
}

Vector operator/(double coefficient, const Vector& vector) {
	Vector result = vector;
	result /= coefficient;
	return result;
}

double crossProduct(const Vector& first, const Vector& second) {
	return first.x * second.y - first.y * second.x;
}

double dotProduct(const Vector& first, const Vector& second) {
	return first.x * second.x + first.y * second.y;
}

Vector turnedLeft(const Vector& vector) {
	return Vector(-vector.y, vector.x);
}

Vector operator-(const Vector& vector) {
	return Vector(-vector.x, -vector.y);
}

Vector turnedRight(const Vector& vector) {
	return Vector(vector.y, -vector.x);
}

Vector turned(const Vector& vector, double angle) {
	double sin = std::sin(angle);
	double cos = std::cos(angle);
	return Vector(vector.x * cos - vector.y * sin, vector.x * sin + vector.y * cos);
}

double distance(const Point& first, const Point& second) {
	return (second - first).length();
}

Vector normalized(const Vector& vector) {
	Vector result = vector;
	result /= vector.length();
	return result;
}


double degreesToRadians(double degrees) {
	return degrees / 180.0 * PI;
}

double radiansToDegrees(double radians) {
	return radians / PI * 180.0;
}


std::vector<double> segmentedLineXs(const SegmentedLine& line) {
	std::vector<double> result;
	for (const Point& point : line) {
		result.push_back(point.x);
	}
	return result;
}

std::vector<double> segmentedLineYs(const SegmentedLine& line) {
	std::vector<double> result;
	for (const Point& point : line) {
		result.push_back(point.y);
	}
	return result;
}