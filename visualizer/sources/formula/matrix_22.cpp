#include "matrix_22.h"

#include <iostream>
#include <sstream>


Matrix22::Matrix22(double corresponding_number) {
	values[0][0] = values[1][1] = corresponding_number;
	values[0][1] = values[1][0] = 0;
}

Matrix22::Matrix22(double value_11, double value_12, double value_21, double value_22) {
	values[0][0] = value_11;
	values[0][1] = value_12;
	values[1][0] = value_21;
	values[1][1] = value_22;
}

Matrix22::Matrix22(const std::vector<double>& values) {
	this->values[0][0] = values[0];
	this->values[0][1] = values[1];
	this->values[1][0] = values[2];
	this->values[1][1] = values[3];
}

Matrix22::Matrix22(const std::string& symbols) {
	values[0][0] = values[0][1] = values[1][0] = values[1][1] = 0;
	std::stringstream stream(symbols);
	char separator;
	stream >> values[0][0];
	stream >> separator >> values[0][1];
	stream >> separator >> values[1][0];
	stream >> separator >> values[1][1];
}

Matrix22& Matrix22::operator+=(const Matrix22& other) {
	values[0][0] += other.values[0][0];
	values[0][1] += other.values[0][1];
	values[1][0] += other.values[1][0];
	values[1][1] += other.values[1][1];
	return *this;
}

Matrix22& Matrix22::operator-=(const Matrix22& other) {
	values[0][0] -= other.values[0][0];
	values[0][1] -= other.values[0][1];
	values[1][0] -= other.values[1][0];
	values[1][1] -= other.values[1][1];
	return *this;
}

Matrix22& Matrix22::operator*=(double coefficient) {
	values[0][0] *= coefficient;
	values[0][1] *= coefficient;
	values[1][0] *= coefficient;
	values[1][1] *= coefficient;
	return *this;
}

Matrix22& Matrix22::operator/=(double coefficient) {
	values[0][0] /= coefficient;
	values[0][1] /= coefficient;
	values[1][0] /= coefficient;
	values[1][1] /= coefficient;
	return *this;
}

double Matrix22::determinant() const {
	return values[0][0] * values[1][1] - values[0][1] * values[1][0];
}

std::string Matrix22::asString() const {
	std::stringstream stream;
	stream << values[0][0] << "; " << values[0][1] << "; " << values[1][0] << "; " << values[1][1];
	return stream.str();
}

void Matrix22::print() const {
	std::cout << "Matrix(\n" << values[0][0] << "; " << values[0][1] << "\n" <<
		values[1][0] << "; " << values[1][1] << "\n)\n";
}


Matrix22 operator+(const Matrix22& first, const Matrix22& second) {
	Matrix22 result(first);
	result += second;
	return result;
}

Matrix22 operator-(const Matrix22& first, const Matrix22& second) {
	Matrix22 result(first);
	result += second;
	return result;
}

Matrix22 operator*(double coefficient, const Matrix22& matrix) {
	Matrix22 result(matrix);
	result *= coefficient;
	return result;
}

Matrix22 matrixProduct(const Matrix22& first, const Matrix22& second) {
	Matrix22 result;
	result.values[0][0] = first.values[0][0] * second.values[0][0] +
		first.values[0][1] * second.values[1][0];
	result.values[0][1] = first.values[0][0] * second.values[0][1] +
		first.values[0][1] * second.values[1][1];
	result.values[1][0] = first.values[1][0] * second.values[0][0] +
		first.values[1][1] * second.values[1][0];
	result.values[1][1] = first.values[1][0] * second.values[0][1] +
		first.values[1][1] * second.values[1][1];
	return result;
}

Vector matrixVectorProduct(const Matrix22& matrix, const Vector& vector) {
	Vector result;
	result.x = matrix.values[0][0] * vector.x + matrix.values[0][1] * vector.y;
	result.y = matrix.values[1][0] * vector.x + matrix.values[1][1] * vector.y;
	return result;
}

Matrix22 transposed(const Matrix22& matrix) {
	Matrix22 result;
	result.values[0][0] = matrix.values[0][0];
	result.values[0][1] = matrix.values[1][0];
	result.values[1][0] = matrix.values[0][1];
	result.values[1][1] = matrix.values[1][1];
	return result;
}

Matrix22 algebraicComplement(const Matrix22& matrix) {
	Matrix22 result;
	result.values[0][0] = matrix.values[1][1];
	result.values[0][1] = -matrix.values[1][0];
	result.values[1][0] = -matrix.values[0][1];
	result.values[1][1] = matrix.values[0][0];
	return result;
}

Matrix22 inverse(const Matrix22& matrix) {
	Matrix22 result = transposed(algebraicComplement(matrix));
	result /= matrix.determinant();
	return result;
}