#pragma once

#include <vector>


extern const double PI;
extern const double EPS;


struct Point;
struct Vector;
using SegmentedLine = std::vector<Point>;


struct Point {
	double x;
	double y;

	Point(double x = 0, double y = 0);
	explicit Point(const Vector&);
	Point& operator+=(const Vector&);
	Point& operator-=(const Vector&);
	bool isFinite() const;
	void print() const;
};

struct Vector {
	double x;
	double y;

	Vector(double x = 0, double y = 0);
	explicit Vector(const Point&);
	Vector& operator+=(const Vector&);
	Vector& operator-=(const Vector&);
	Vector& operator*=(double);
	Vector& operator/=(double);
	double lengthSquared() const;
	double length() const;
	bool isFinite() const;
	void print() const;
};

Point operator+(const Point&, const Vector&);
Point operator-(const Point&, const Vector&);
Vector operator-(const Point&, const Point&);
Vector operator+(const Vector&, const Vector&);
Vector operator-(const Vector&, const Vector&);
Vector operator*(double, const Vector&);
double crossProduct(const Vector&, const Vector&);
double dotProduct(const Vector&, const Vector&);
Vector turnedLeft(const Vector&);
Vector operator-(const Vector&);
Vector turnedRight(const Vector&);
Vector turned(const Vector&, double);
double distanceBetweenSquared(const Point&, const Point&);
double distanceBetween(const Point&, const Point&);
Vector normalized(const Vector&);

double degreesToRadians(double);
double radiansToDegrees(double);

double sign(double);


std::vector<double> segmentedLineXs(const SegmentedLine&);
std::vector<double> segmentedLineYs(const SegmentedLine&);