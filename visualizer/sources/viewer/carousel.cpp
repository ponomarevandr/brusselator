#include "carousel.h"

#include "tracker/tracker.h"
#include "tracker/visual_preparator.h"

#include <fstream>


Carousel::ElementBase::ElementBase(size_t formulas_number): labels(formulas_number),
	formulas(formulas_number), coordinates_matrix(1.0) {}

Carousel::Portrait Carousel::ElementBase::getPortrait(const Frame& zone, double step,
		double max_between_tracks, double min_between_tracks) const {
	if (!is_active)
		return std::make_pair(std::vector<SegmentedLine>(), color);	
	VectorField field = getFieldForPortrait();
	Tracker tracker(field, zone, step, max_between_tracks, min_between_tracks);
	std::vector<SegmentedLine> tracks = tracker.getTracks();
	VisualPreparator preparator(tracks, zone, true);
	preparator.prepareTracks();
	return std::make_pair(tracks, color);
}

bool Carousel::ElementBase::isValid() const {
	if (!is_active)
		return true;
	for (const FormulaXY& formula : formulas) {
		if (!formula.isValid())
			return false;
	}
	return coordinates_matrix.determinant() != 0;
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

bool Carousel::ElementBase::getIsActive() const {
	return is_active;
}

void Carousel::ElementBase::setIsActive(bool value) {
	is_active = value;
}

Matrix22 Carousel::ElementBase::getCoordinatesMatrix() const {
	return coordinates_matrix;
}

void Carousel::ElementBase::setCoordinatesMatrix(const Matrix22& matrix) {
	coordinates_matrix = matrix;
}

void Carousel::ElementBase::serialize(std::ofstream& fout) const {
	fout << static_cast<size_t>(getType()) << "\n";
	fout << labels.size() << "\n";
	for (size_t i = 0; i < labels.size(); ++i) {
		fout << labels[i] << "\n";
		fout << formulas[i].getSymbols() << "\n";
	}
	fout << static_cast<size_t>(color) << "\n";
	fout << is_active << "\n";
}

void Carousel::ElementBase::deserialize(std::ifstream& fin) {
	size_t formulas_number;
	fin >> formulas_number;
	std::string tmp;
	std::getline(fin, tmp);
	labels.resize(formulas_number);
	formulas.resize(formulas_number);
	for (size_t i = 0; i < formulas_number; ++i) {
		std::getline(fin, labels[i]);
		std::string symbols;
		std::getline(fin, symbols);
		formulas[i] = FormulaXY(symbols);
	}
	size_t color_as_number;
	fin >> color_as_number;
	color = static_cast<Plotter::Color>(color_as_number);
	fin >> is_active;
}


Carousel::ElementSystem::ElementSystem(): ElementBase(2) {
	labels[0] = "x' =";
	labels[1] = "y' =";
}

VectorField Carousel::ElementSystem::getFieldForPortrait() const {
	return VectorField(formulas[0], formulas[1]);
}

double Carousel::ElementSystem::getFunctionValue(Point point) const {
	return 0.0;
};

Carousel::ElementType Carousel::ElementSystem::getType() const {
	return ElementType::SYSTEM;
}


Carousel::ElementLevels::ElementLevels(): ElementBase(1) {
	labels[0] = "= f =";
}

VectorField Carousel::ElementLevels::getFieldForPortrait() const {
	FormulaXY df_dx = derivativeX(formulas[0]);
	FormulaXY df_dy = derivativeY(formulas[0]);
	return VectorField(unaryOperation('-', df_dy), df_dx);
}

double Carousel::ElementLevels::getFunctionValue(Point point) const {
	if (!is_active || !isValid())
		return 0.0;
	return formulas[0](point.x, point.y);
};

Carousel::ElementType Carousel::ElementLevels::getType() const {
	return ElementType::LEVELS;
}


Carousel::ElementTendency::ElementTendency(): ElementBase(1) {
	labels[0] = "<- f =";
}

VectorField Carousel::ElementTendency::getFieldForPortrait() const {
	FormulaXY df_dx = derivativeX(formulas[0]);
	FormulaXY df_dy = derivativeY(formulas[0]);
	FormulaXY minus_df_dy = unaryOperation('-', df_dy);
	FormulaXY vx(binaryOperation(minus_df_dy, '-', binaryOperation(formulas[0], '*', df_dx)));
	FormulaXY vy(binaryOperation(df_dx, '-', binaryOperation(formulas[0], '*', df_dy)));
	return VectorField(vx, vy);
}

double Carousel::ElementTendency::getFunctionValue(Point point) const {
	if (!is_active || !isValid())
		return 0.0;
	return formulas[0](point.x, point.y);
};

Carousel::ElementType Carousel::ElementTendency::getType() const {
	return ElementType::TENDENCY;
}

Carousel::ElementDivergency::ElementDivergency(): ElementBase(3) {}

FormulaXY Carousel::ElementDivergency::getDivergency() const {
	FormulaXY multiplied_x(binaryOperation(formulas[2], '*', formulas[0]));
	FormulaXY multiplied_y(binaryOperation(formulas[2], '*', formulas[1]));
	return binaryOperation(derivativeX(multiplied_x), '+', derivativeY(multiplied_y));
}


Carousel::ElementDivergencyLevels::ElementDivergencyLevels(): ElementDivergency() {
	labels[0] = "= x' =";
	labels[1] = "= y' =";
	labels[2] = "mu =";
}

VectorField Carousel::ElementDivergencyLevels::getFieldForPortrait() const {
	FormulaXY divergency = getDivergency();
	FormulaXY df_dx = derivativeX(divergency);
	FormulaXY df_dy = derivativeY(divergency);
	FormulaXY minus_df_dy(unaryOperation('-', df_dy));
	return VectorField(minus_df_dy, df_dx);
}

double Carousel::ElementDivergencyLevels::getFunctionValue(Point point) const {
	if (!is_active || !isValid())
		return 0.0;
	return getDivergency()(point.x, point.y);
};

Carousel::ElementType Carousel::ElementDivergencyLevels::getType() const {
	return ElementType::DIV_LEVELS;
}


Carousel::ElementDivergencyTendency::ElementDivergencyTendency(): ElementDivergency() {
	labels[0] = "<- x' =";
	labels[1] = "<- y' =";
	labels[2] = "mu =";
}

VectorField Carousel::ElementDivergencyTendency::getFieldForPortrait() const {
	FormulaXY divergency = getDivergency();
	FormulaXY df_dx = derivativeX(divergency);
	FormulaXY df_dy = derivativeY(divergency);
	FormulaXY minus_df_dy(unaryOperation('-', df_dy));
	FormulaXY vx(binaryOperation(minus_df_dy, '-', binaryOperation(divergency, '*', df_dx)));
	FormulaXY vy(binaryOperation(df_dx, '-', binaryOperation(divergency, '*', df_dy)));
	return VectorField(vx, vy);
}

double Carousel::ElementDivergencyTendency::getFunctionValue(Point point) const {
	if (!is_active || !isValid())
		return 0.0;
	return getDivergency()(point.x, point.y);
};

Carousel::ElementType Carousel::ElementDivergencyTendency::getType() const {
	return ElementType::DIV_TENDENCY;
}


std::unique_ptr<Carousel::ElementBase> Carousel::constructElement(ElementType type) {
	switch (type) {
	case ElementType::SYSTEM:
		return std::make_unique<ElementSystem>();
	case ElementType::LEVELS:
		return std::make_unique<ElementLevels>();
	case ElementType::TENDENCY:
		return std::make_unique<ElementTendency>();
	case ElementType::DIV_LEVELS:
		return std::make_unique<ElementDivergencyLevels>();
	case ElementType::DIV_TENDENCY:
		return std::make_unique<ElementDivergencyTendency>();
	default:
		return nullptr;
	}
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
	elements.insert(elements.begin() + index + 1, constructElement(type));
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

bool Carousel::getIsActive() const {
	return elements[index]->getIsActive();
}

void Carousel::setIsActive(bool value) {
	return elements[index]->setIsActive(value);
}

Matrix22 Carousel::getCoordinatesMatrix() const {
	return elements[index]->getCoordinatesMatrix();
}

void Carousel::setCoordinatesMatrix(const Matrix22& matrix) {
	elements[index]->setCoordinatesMatrix(matrix);
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

double Carousel::getFunctionValue(Point point) const {
	return elements[index]->getFunctionValue(point);
}

bool Carousel::loadFromFile(const std::string& filename) {
	std::ifstream fin(filename);
	if (!fin.is_open())
		return false;
	size_t elements_number;
	fin >> elements_number;
	elements.resize(elements_number);
	for (size_t i = 0; i < elements.size(); ++i) {
		ElementType type;
		size_t type_as_number;
		fin >> type_as_number;
		type = static_cast<ElementType>(type_as_number);
		elements[i] = constructElement(type);
		elements[i]->deserialize(fin);
	}
	return true;
}

void Carousel::saveToFile(const std::string& filename) const {
	std::ofstream fout(filename);
	fout << elements.size() << "\n";
	for (size_t i = 0; i < elements.size(); ++i) {
		elements[i]->serialize(fout);
	}
	fout.close();
}