#pragma once

#include <string>
#include <functional>

#include "TFormula.h"


class FormulaXY {
private:
	static const double START_DARG;
	static const double STEP_DARG;
	static const size_t MAX_STEPS;
	std::string symbols;
	TFormula implementation;

private:
	static double getDerivative(double x, std::function<double(double)> f);

public:
	FormulaXY() = default;
	FormulaXY(const std::string& symbols);
	bool isValid() const;
	double operator()(double x, double y) const;
	std::string getSymbols() const;
	double derivativeX(double x, double y) const;
	double derivativeY(double x, double y) const;
};