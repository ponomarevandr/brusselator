#pragma once

#include "geometry/basics.h"

#include <vector>
#include <string>


class Matrix22 {
public:
	double values[2][2];

public:
	explicit Matrix22(double corresponding_number = 0);
	Matrix22(double value_11, double value_12, double value_21, double value_22);
	explicit Matrix22(const std::vector<double>& values);
	explicit Matrix22(const std::string& symbols);
	Matrix22& operator+=(const Matrix22&);
	Matrix22& operator-=(const Matrix22&);
	Matrix22& operator*=(double);
	Matrix22& operator/=(double);
	double determinant() const;
	std::string asString() const;
	void print() const;
};

Matrix22 operator+(const Matrix22&, const Matrix22&);
Matrix22 operator-(const Matrix22&, const Matrix22&);
Matrix22 operator*(double, const Matrix22&);
Matrix22 matrixProduct(const Matrix22&, const Matrix22&);
Vector matrixVectorProduct(const Matrix22&, const Vector&);
Matrix22 transposed(const Matrix22&);
Matrix22 algebraicComplement(const Matrix22&);
Matrix22 inverse(const Matrix22&);