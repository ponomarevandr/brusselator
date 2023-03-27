#include "formula_xy.h"

#include <cstdlib>
#include <matheval.h>


FormulaXY::FormulaXY(void* evaluator): evaluator(evaluator) {
	symbols = std::string(evaluator_get_string(evaluator));
}

FormulaXY::FormulaXY(const std::string& symbols): symbols(symbols),
	evaluator(evaluator_create(const_cast<char*>(symbols.c_str()))) {}

FormulaXY::~FormulaXY() {
	if (evaluator)
		evaluator_destroy(evaluator);
}

FormulaXY::FormulaXY(const FormulaXY& other): symbols(other.symbols),
	evaluator(evaluator_create(const_cast<char*>(other.symbols.c_str()))) {}

FormulaXY::FormulaXY(FormulaXY&& other): symbols(std::move(other.symbols)),
		evaluator(other.evaluator) {
	other.symbols.clear();
	other.evaluator = nullptr;
}

FormulaXY& FormulaXY::operator=(const FormulaXY& other) {
	symbols = other.symbols;
	evaluator = evaluator_create(const_cast<char*>(other.symbols.c_str()));
	return *this;
}

FormulaXY& FormulaXY::operator=(FormulaXY&& other) {
	symbols = std::move(other.symbols);
	evaluator = other.evaluator;
	other.symbols.clear();
	other.evaluator = nullptr;
	return *this;
}

bool FormulaXY::isValid() const {
	if (!evaluator)
		return false;
	char** variables_names;
	int variables_count;
	evaluator_get_variables(evaluator, &variables_names, &variables_count);
	for (int i = 0; i < variables_count; ++i) {
		std::string name(variables_names[i]);
		if (name != "x" && name != "y")
			return false;
	}
	return true;
}

double FormulaXY::operator()(double x, double y) const {
	return evaluator_evaluate_x_y(evaluator, x, y);
}

std::string FormulaXY::getSymbols() const {
	return symbols;
}


FormulaXY derivativeX(const FormulaXY& function) {
	void* evaluator = evaluator_derivative_x(function.evaluator);
	return FormulaXY(evaluator);
}

FormulaXY derivativeY(const FormulaXY& function) {
	void* evaluator = evaluator_derivative_y(function.evaluator);
	return FormulaXY(evaluator);
}

FormulaXY divergency(const FormulaXY& function) {
	void* evaluator_x = evaluator_derivative_x(function.evaluator);
	void* evaluator_y = evaluator_derivative_y(function.evaluator);
	std::string symbols_derivative_x = std::string(evaluator_get_string(evaluator_x));
	std::string symbols_derivative_y = std::string(evaluator_get_string(evaluator_y));
	evaluator_destroy(evaluator_x);
	evaluator_destroy(evaluator_y);
	return FormulaXY(symbols_derivative_x + " + " + symbols_derivative_y);
}