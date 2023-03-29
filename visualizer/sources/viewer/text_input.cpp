#include "text_input.h"

#include <sstream>


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
void TextInput<double>::setValue(const double& value) {
	std::stringstream stream;
	stream << value;
	input->value(stream.str().c_str());
	input->redraw();
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
void TextInput<std::string>::setValue(const std::string& value) {
	input->value(value.c_str());
	input->redraw();
}