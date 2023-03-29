#pragma once

#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>

#include <string>
#include <memory>


template <typename Type>
class TextInput {
private:
	std::string label_text;
	std::unique_ptr<Fl_Box> label;
	std::unique_ptr<Fl_Input> input;

public:
	TextInput() = default;
	TextInput& operator=(const TextInput&) = delete;
	TextInput(const TextInput&) = delete;

	TextInput(size_t x, size_t y, size_t width, size_t height, size_t label_width,
			const std::string& text = ""): label_text(text) {
		label = std::make_unique<Fl_Box>(FL_NO_BOX, x, y, label_width, height, label_text.c_str());
		input = std::make_unique<Fl_Input>(x + label_width, y, width - label_width, height);
	};

	TextInput(TextInput&& other): label_text(std::move(label_text)), label(std::move(label)),
			input(std::move(input)) {
		label->label(label_text.c_str());
		label->redraw();
	}

	TextInput& operator=(TextInput&& other) {
		label_text = std::move(other.label_text);
		label = std::move(other.label);
		input = std::move(other.input);
		label->label(label_text.c_str());
		label->redraw();
		return *this;
	}

	void setLabel(const std::string&);
	bool isValid() const;
	Type getValue() const;
	void setValue(const Type&);
};