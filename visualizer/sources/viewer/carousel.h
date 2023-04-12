#pragma once

#include "geometry/basics.h"
#include "geometry/frame.h"
#include "plotter/plotter.h"
#include "formula/formula_xy.h"
#include "formula/matrix_22.h"
#include "geometry/vector_field.h"

#include <vector>
#include <string>
#include <memory>


class Carousel {
public:
	using Portrait = std::pair<std::vector<SegmentedLine>, Plotter::Color>;
	enum class ElementType {
		SYSTEM,
		LEVELS,
		TENDENCY,
		DIV_LEVELS,
		DIV_TENDENCY
	};

private:
	class ElementBase {
	protected:
		std::vector<std::string> labels;
		std::vector<FormulaXY> formulas;
		Plotter::Color color = Plotter::Color::RED;
		bool is_active = true;
		Matrix22 coordinates_matrix;

	protected:
		virtual VectorField getFieldForPortrait() const = 0;
		static FormulaXY getSubstitutionX(const Matrix22&);
		static FormulaXY getSubstitutionY(const Matrix22&);
		static FormulaXY matrixSubstitution(const FormulaXY&, const Matrix22&);

	public:
		ElementBase(size_t formulas_number);
		virtual ~ElementBase() = default;
		virtual double getFunctionValue(Point) const = 0;
		virtual Carousel::ElementType getType() const = 0;
		Portrait getPortrait(const Frame& zone, double step, double max_between_tracks,
			double min_between_tracks) const;
		bool isValid() const;
		size_t getFormulasNumber() const;
		std::vector<std::string> getLabels() const;
		std::vector<std::string> getFormulasSymbols() const;
		void setFormulaSymbols(size_t index, const std::string& symbols);
		Plotter::Color getColor() const;
		void setColor(Plotter::Color);
		bool getIsActive() const;
		void setIsActive(bool);
		Matrix22 getCoordinatesMatrix() const;
		void setCoordinatesMatrix(const Matrix22&);
		void serialize(std::ofstream&) const;
		void deserialize(std::ifstream&);
	};

	class ElementSystem: public ElementBase {
	protected:
		virtual VectorField getFieldForPortrait() const override;

	public:
		ElementSystem();
		virtual double getFunctionValue(Point) const override;
		virtual Carousel::ElementType getType() const override;
	};

	class ElementLevels: public ElementBase {
	protected:
		virtual VectorField getFieldForPortrait() const override;

	public:
		ElementLevels();
		virtual double getFunctionValue(Point) const override;
		virtual Carousel::ElementType getType() const override;
	};

	class ElementTendency: public ElementBase {
	protected:
		virtual VectorField getFieldForPortrait() const override;

	public:
		ElementTendency();
		virtual double getFunctionValue(Point) const override;
		virtual Carousel::ElementType getType() const override;
	};

	class ElementDivergency: public ElementBase {
	protected:
		FormulaXY getDivergency() const;

	public:
		ElementDivergency();
	};

	class ElementDivergencyLevels: public ElementDivergency {
	protected:
		virtual VectorField getFieldForPortrait() const override;

	public:
		ElementDivergencyLevels();
		virtual double getFunctionValue(Point) const override;
		virtual Carousel::ElementType getType() const override;
	};

	class ElementDivergencyTendency: public ElementDivergency {
	protected:
		virtual VectorField getFieldForPortrait() const override;

	public:
		ElementDivergencyTendency();
		virtual double getFunctionValue(Point) const override;
		virtual Carousel::ElementType getType() const override;
	};

private:
	size_t index = 0;
	std::vector<std::unique_ptr<ElementBase>> elements;

private:
	static std::unique_ptr<ElementBase> constructElement(ElementType);

public:
	Carousel();
	void toNext();
	void toPrevious();
	void addElement(ElementType);
	void removeElement();
	bool isValid() const;
	size_t getIndex() const;
	size_t getFormulasNumber() const;
	std::vector<std::string> getLabels() const;
	std::vector<std::string> getFormulasSymbols() const;
	void setFormulaSymbols(size_t index_in_element, const std::string& symbols);
	Plotter::Color getColor() const;
	void setColor(Plotter::Color);
	bool getIsActive() const;
	void setIsActive(bool);
	Matrix22 getCoordinatesMatrix() const;
	void setCoordinatesMatrix(const Matrix22&);
	std::vector<Portrait> getPortraits(const Frame& zone, double step, double max_between_tracks,
		double min_between_tracks) const;
	double getFunctionValue(Point) const;
	bool loadFromFile(const std::string& filename);
	void saveToFile(const std::string& filename) const;
};