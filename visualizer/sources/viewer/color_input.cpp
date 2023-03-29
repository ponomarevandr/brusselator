#include "color_input.h"


const int ColorInput::FLTK_COLOR[7] = {
	FL_RED, FL_YELLOW, FL_GREEN, FL_CYAN, FL_BLUE, FL_MAGENTA, FL_BLACK
};

void ColorInput::buttonCallback(Fl_Widget* widget, void* ptr) {
	Plotter::Color color = static_cast<ColorInput*>(ptr)->color;
	color = static_cast<Plotter::Color>((static_cast<size_t>(color) + 1) % 6);
	static_cast<ColorInput*>(ptr)->setColor(color);
}

ColorInput::ColorInput(size_t x, size_t y, size_t width, size_t height, const std::string& text):
		button_text(text) {
	button = std::make_unique<Fl_Button>(x, y, width, height, button_text.c_str());
	button->callback(ColorInput::buttonCallback, this);
	setColor(Plotter::Color::RED);
}

ColorInput::ColorInput(ColorInput&& other): button_text(std::move(other.button_text)),
		button(std::move(other.button)) {
	button->callback(ColorInput::buttonCallback, this);
	button->label(button_text.c_str());
	setColor(other.color);
}

ColorInput& ColorInput::operator=(ColorInput&& other) {
	button_text = std::move(other.button_text);
	button = std::move(other.button);
	button->callback(ColorInput::buttonCallback, this);
	button->label(button_text.c_str());
	setColor(other.color);
	return *this;
}

Plotter::Color ColorInput::getColor() const {
	return color;
}

void ColorInput::setColor(Plotter::Color color) {
	this->color = color;
	button->color(FLTK_COLOR[static_cast<size_t>(color)]);
	button->redraw();
}