#include "carousel.h"

#include "tracker/tracker.h"
#include "tracker/visual_preparator.h"


Carousel::ElementBase::ElementBase(size_t formulas_number): labels(formulas_number),
	formulas(formulas_number) {}

Carousel::Portrait Carousel::ElementBase::getPortrait(const Frame& zone, double step,
		double max_between_tracks, double min_between_tracks) const {
	VectorField field = getFieldForPortrait();
	Tracker tracker(field, zone, step, max_between_tracks, min_between_tracks);
	std::vector<SegmentedLine> tracks = tracker.getTracks();
	VisualPreparator preparator(tracks, zone, true);
	preparator.prepareTracks();
	return std::make_pair(tracks, color);
}

bool Carousel::ElementBase::isValid() const {
	for (const FormulaXY& formula : formulas) {
		if (!formula.isValid())
			return false;
	}
	return true;
}

size_t Carousel::ElementBase::getFormulasNumber() const {
	return labels.size();
}

std::vector<std::string> Carousel::ElementBase::getLabels() const {
	return labels;
}

std::vector<std::string> Carousel::ElementBase::getFormulasSymbols() const {
	std::vector<std::string> result;
	for (const FormulaXY& formula : formulas) {
		result.push_back(formula.getSymbols());
	}
	return result;
}

void Carousel::ElementBase::setFormulaSymbols(size_t index, const std::string& symbols) {
	formulas[index] = FormulaXY(symbols);
}

Plotter::Color Carousel::ElementBase::getColor() const {
	return color;
}

void Carousel::ElementBase::setColor(Plotter::Color color) {
	this->color = color;
}


Carousel::ElementSystem::ElementSystem(): ElementBase(2) {
	labels[0] = "x' =";
	labels[1] = "y' =";
}

VectorField Carousel::ElementSystem::getFieldForPortrait() const {
	return VectorField(formulas[0], formulas[1]);
}


Carousel::ElementLevels::ElementLevels(): ElementBase(1) {
	labels[0] = "=f = ";
}

VectorField Carousel::ElementLevels::getFieldForPortrait() const {
	FormulaXY df_dx = derivativeX(formulas[0]);
	FormulaXY df_dy = derivativeY(formulas[0]);
	FormulaXY minus_df_dy(std::string("-(") + df_dy.getSymbols() + ")");
	return VectorField(minus_df_dy, df_dx);
}


Carousel::ElementTendency::ElementTendency(): ElementBase(1) {
	labels[0] = "<-f = ";
}

VectorField Carousel::ElementTendency::getFieldForPortrait() const {
	FormulaXY df_dx = derivativeX(formulas[0]);
	FormulaXY df_dy = derivativeY(formulas[0]);
	FormulaXY minus_df_dy(std::string("-(") + df_dy.getSymbols() + ")");
	FormulaXY vx(minus_df_dy.getSymbols() + " - (" + formulas[0].getSymbols() + ") * (" +
		df_dx.getSymbols() + ")");
	FormulaXY vy(df_dx.getSymbols() + " - (" + formulas[0].getSymbols() + ") * (" +
		df_dy.getSymbols() + ")");
	return VectorField(vx, vy);
}


Carousel::ElementDivergency::ElementDivergency(): ElementBase(3) {
	labels[0] = "x' = ";
	labels[1] = "y' = ";
	labels[2] = "mu = ";
}

VectorField Carousel::ElementDivergency::getFieldForPortrait() const {
	FormulaXY multiplied_x(std::string("(") + formulas[2].getSymbols() + ")*(" +
		formulas[0].getSymbols() + ")");
	FormulaXY multiplied_y(std::string("(") + formulas[2].getSymbols() + ")*(" +
		formulas[1].getSymbols() + ")");
	FormulaXY divergency(derivativeX(multiplied_x).getSymbols() + " + " +
		derivativeY(multiplied_y).getSymbols());
	FormulaXY df_dx = derivativeX(divergency);
	FormulaXY df_dy = derivativeY(divergency);
	FormulaXY minus_df_dy(std::string("-(") + df_dy.getSymbols() + ")");
	FormulaXY vx(minus_df_dy.getSymbols() + " - (" + divergency.getSymbols() + ") * (" +
		df_dx.getSymbols() + ")");
	FormulaXY vy(df_dx.getSymbols() + " - (" + divergency.getSymbols() + ") * (" +
		df_dy.getSymbols() + ")");
	return VectorField(vx, vy);
}


Carousel::Carousel() {
	elements.push_back(std::make_unique<ElementSystem>());
}

void Carousel::toNext() {
	index = (index + 1) % elements.size();
}

void Carousel::toPrevious() {
	index = (index + elements.size() - 1) % elements.size();
}

void Carousel::addElement(ElementType type) {
	std::unique_ptr<ElementBase> element;
	switch (type) {
	case ElementType::SYSTEM:
		element = std::make_unique<ElementSystem>();
		break;
	case ElementType::LEVELS:
		element = std::make_unique<ElementLevels>();
		break;
	case ElementType::TENDENCY:
		element = std::make_unique<ElementTendency>();
		break;
	case ElementType::DIVERGENCY:
		element = std::make_unique<ElementDivergency>();
		break;
	}
	elements.insert(elements.begin() + index + 1, std::move(element));
	toNext();
}

void Carousel::removeElement() {
	elements.erase(elements.begin() + index);
	if (elements.empty())
		elements.push_back(std::make_unique<ElementSystem>());
	toPrevious();
	toNext();
}

bool Carousel::isValid() const {
	for (size_t i = 0; i < elements.size(); ++i) {
		if (!elements[i]->isValid())
			return false;
	}
	return true;
}

size_t Carousel::getIndex() const {
	return index;
}

size_t Carousel::getFormulasNumber() const {
	return elements[index]->getFormulasNumber();
}

std::vector<std::string> Carousel::getLabels() const {
	return elements[index]->getLabels();
}

std::vector<std::string> Carousel::getFormulasSymbols() const {
	return elements[index]->getFormulasSymbols();
}

void Carousel::setFormulaSymbols(size_t index_in_element, const std::string& symbols) {
	elements[index]->setFormulaSymbols(index_in_element, symbols);
}

Plotter::Color Carousel::getColor() const {
	return elements[index]->getColor();
}

void Carousel::setColor(Plotter::Color color) {
	elements[index]->setColor(color);
}

std::vector<Carousel::Portrait> Carousel::getPortraits(const Frame& zone, double step,
		double max_between_tracks, double min_between_tracks) const {
	std::vector<Portrait> result;
	for (size_t i = 0; i < elements.size(); ++i) {
		result.push_back(elements[i]->getPortrait(zone, step, max_between_tracks,
			min_between_tracks));
	}
	return result;
}