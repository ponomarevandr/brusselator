#pragma once

#include <string>

#include "TFormula.h"


class FormulaXY {
private:
	std::string symbols;
	TFormula implementation;

public:
	FormulaXY() = default;
	FormulaXY(const std::string& symbols);
	bool isValid() const;
	double operator()(double x, double y) const;
	std::string getSymbols() const;
};