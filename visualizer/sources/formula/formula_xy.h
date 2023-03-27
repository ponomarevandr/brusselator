#pragma once

#include <string>
#include <functional>


class FormulaXY {
	friend FormulaXY derivativeX(const FormulaXY&);
	friend FormulaXY derivativeY(const FormulaXY&);
	friend FormulaXY divergency(const FormulaXY&);

private:
	std::string symbols;
	void* evaluator = nullptr;

private:
	FormulaXY(void* evaluator);

public:
	FormulaXY() = default;
	explicit FormulaXY(const std::string& symbols);
	~FormulaXY();
	FormulaXY(const FormulaXY&);
	FormulaXY(FormulaXY&&);
	FormulaXY& operator=(const FormulaXY&);
	FormulaXY& operator=(FormulaXY&&);

	bool isValid() const;
	double operator()(double x, double y) const;
	std::string getSymbols() const;
};

FormulaXY derivativeX(const FormulaXY&);
FormulaXY derivativeY(const FormulaXY&);
FormulaXY divergency(const FormulaXY&);