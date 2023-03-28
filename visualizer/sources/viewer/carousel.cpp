#include "carousel.h"

#include "geometry/vector_field.h"
#include "tracker/tracker.h"
#include "tracker/visual_preparator.h"


Carousel::ElementBase::ElementBase(size_t formulas_number): labels(formulas_number),
	formulas(formulas_number) {}

bool Carousel::ElementBase::isValid() const {
	for (const FormulaXY& formula : formulas) {
		if (!formula.isValid())
			return false;
	}
	return true;
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
	labels[0] = "x' = ";
	labels[1] = "y' = ";
}

Carousel::Portrait Carousel::ElementSystem::getPortrait(const Frame& zone) const {
	VectorField field(formulas[0], formulas[1]);
	Tracker tracker(field, zone);
	std::vector<SegmentedLine> tracks = tracker.getTracks();
	VisualPreparator preparator(tracks, zone, true);
	preparator.prepareTracks();
	return std::make_pair(tracks, color);
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

void Carousel::addElement() {
	elements.insert(elements.begin() + index, std::make_unique<ElementSystem>());
}

void Carousel::removeElement() {
	elements.erase(elements.begin() + index);
	if (elements.empty())
		elements.push_back(std::make_unique<ElementSystem>());
	toPrevious();
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

std::vector<Carousel::Portrait> Carousel::getPortraits(const Frame& zone) const {
	std::vector<Portrait> result;
	for (size_t i = 0; i < elements.size(); ++i) {
		result.push_back(elements[i]->getPortrait(zone));
	}
	return result;
}