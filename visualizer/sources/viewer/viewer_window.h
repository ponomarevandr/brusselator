#pragma once

#include "geometry/basics.h"
#include "geometry/frame.h"
#include "plotter/plotter.h"
#include "viewer/carousel.h"
#include "viewer/text_input.h"
#include "viewer/color_input.h"

#include <vector>
#include <string>
#include <memory>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>


class ViewerWindow: public Fl_Double_Window {
private:
	Plotter::Image graph_image;
	std::unique_ptr<Fl_RGB_Image> graph_fltk_image;
	std::unique_ptr<Fl_Box> graph_box;
	TextInput<double> step_input;
	TextInput<double> between_input;
	std::unique_ptr<Fl_Button> movement_button;
	std::unique_ptr<Fl_Button> redraw_button;
	std::unique_ptr<Fl_Button> save_button;
	std::vector<std::unique_ptr<Fl_Box>> formula_labels;
	std::vector<std::unique_ptr<Fl_Input>> formula_inputs;
	std::unique_ptr<Fl_Button> carousel_previous_button;
	std::unique_ptr<Fl_Button> carousel_next_button;
	std::unique_ptr<Fl_Box> carousel_index;
	ColorInput carousel_color_input;
	std::unique_ptr<Fl_Button> carousel_add_button;
	std::unique_ptr<Fl_Button> carousel_remove_button;
	std::vector<std::string> formula_labels_text;
	std::string carousel_index_text;
	Point zone_center;
	Vector zone_to_corner;
	std::vector<Carousel::Portrait> portraits;
	Carousel carousel;

private:
	static void redrawButtonCallback(Fl_Widget* widget, void* ptr);
	static void saveButtonCallback(Fl_Widget* widget, void* ptr);
	static void carouselPreviousButtonCallback(Fl_Widget* widget, void* ptr);
	static void carouselNextButtonCallback(Fl_Widget* widget, void* ptr);
	static void carouselColorButtonCallback(Fl_Widget* widget, void* ptr);
	static void carouselAddButtonCallback(Fl_Widget* widget, void* ptr);
	static void carouselRemoveButtonCallback(Fl_Widget* widget, void* ptr);
	int handle(int event) override;
	void rebuildTracks();
	void saveToCarousel();
	void loadFromCarousel();

public:
	ViewerWindow();
	void redrawImage();
	bool saveImage(const std::string& filename) const;
};