#include "formula_xy.h"


FormulaXY::FormulaXY(const std::string& symbols): symbols(symbols),
	implementation("", symbols.c_str()) {}

bool FormulaXY::isValid() const {
	return implementation.IsValid();
}

double FormulaXY::operator()(double x, double y) const {
	return implementation.Eval(x, y);
}

std::string FormulaXY::getSymbols() const {
	return symbols;
}