#pragma once

#include "geometry/basics.h"
#include "geometry/frame.h"
#include "plotter/plotter.h"
#include "formula/formula_xy.h"

#include <vector>
#include <string>
#include <memory>


class Carousel {
public:
	using Portrait = std::pair<std::vector<SegmentedLine>, Plotter::Color>;

private:
	class ElementBase {
	protected:
		std::vector<std::string> labels;
		std::vector<FormulaXY> formulas;
		Plotter::Color color = Plotter::Color::RED;

	public:
		ElementBase(size_t formulas_number);
		virtual Portrait getPortrait(const Frame& zone) const = 0;
		bool isValid() const;
		std::vector<std::string> getLabels() const;
		std::vector<std::string> getFormulasSymbols() const;
		void setFormulaSymbols(size_t index, const std::string& symbols);
		Plotter::Color getColor() const;
		void setColor(Plotter::Color);
	};

	class ElementSystem: public ElementBase {
	public:
		ElementSystem();
		Portrait getPortrait(const Frame& zone) const override;
	};

private:
	size_t index = 0;
	std::vector<std::unique_ptr<ElementBase>> elements;

public:
	Carousel();
	void toNext();
	void toPrevious();
	void addElement();
	void removeElement();
	bool isValid() const;
	size_t getIndex() const;
	std::vector<std::string> getLabels() const;
	std::vector<std::string> getFormulasSymbols() const;
	void setFormulaSymbols(size_t index_in_element, const std::string& symbols);
	Plotter::Color getColor() const;
	void setColor(Plotter::Color);
	std::vector<Portrait> getPortraits(const Frame& zone) const;
};