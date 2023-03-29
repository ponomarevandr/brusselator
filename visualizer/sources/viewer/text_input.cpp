#include "text_input.h"

#include <sstream>


template <typename Type>
void TextInput<Type>::setLabel(const std::string& text) {
	label_text = text;
	label->label(label_text.c_str());
	label->redraw();
}


template <>
bool TextInput<double>::isValid() const {
	try {
		std::stod(input->value());
	}
	catch (...) {
		return false;
	}
	return true;
}

template <>
double TextInput<double>::getValue() const {
	return std::stod(input->value());
}


template <>
bool TextInput<std::string>::isValid() const {
	return true;
}

template <>
std::string TextInput<std::string>::getValue() const {
	return input->value();
}

template <>
void TextInput<double>::setValue(const double& value) {
	std::stringstream stream;
	stream << value;
	input->value(stream.str().c_str());
	input->redraw();
}