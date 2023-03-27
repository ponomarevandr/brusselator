#include "formula_xy.h"


const double FormulaXY::START_DARG = 1.0;
const double FormulaXY::STEP_DARG = 0.01;
const size_t FormulaXY::MAX_STEPS = 4;

double FormulaXY::getDerivative(double x, std::function<double(double)> f) {
	double fx = f(x);
	double dx = START_DARG;
	double result = 0.0;
	for (size_t i = 0; i < MAX_STEPS; ++i) {
		double current = (f(x + dx) - fx) / dx;
		if (result != 0.0 && current == 0.0)
			break;
		result = current;
		dx *= STEP_DARG;
	}
	return result;
}

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

double FormulaXY::derivativeX(double x, double y) const {
	return getDerivative(x, [this, y](double x) -> double {
		return this->implementation.Eval(x, y);
	});
}

double FormulaXY::derivativeY(double x, double y) const {
	return getDerivative(y, [this, x](double y) -> double {
		return this->implementation.Eval(x, y);
	});
}