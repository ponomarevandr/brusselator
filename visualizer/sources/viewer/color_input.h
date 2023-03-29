#pragma once

#include "plotter/plotter.h"

#include <memory>
#include <string>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>


class ColorInput {
private:
	std::string button_text;
	std::unique_ptr<Fl_Button> button;
	Plotter::Color color;

public:
	static const int FLTK_COLOR[7];

private:
	static void buttonCallback(Fl_Widget* widget, void* ptr);

public:
	ColorInput() = default;
	ColorInput(size_t x, size_t y, size_t width, size_t height, const std::string& text = "");
	ColorInput(const ColorInput&) = delete;
	ColorInput(ColorInput&&);
	ColorInput& operator=(const ColorInput&) = delete;
	ColorInput& operator=(ColorInput&&);
	Plotter::Color getColor() const;
	void setColor(Plotter::Color);
};