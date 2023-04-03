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
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Input.H>


class ViewerWindow: public Fl_Double_Window {
private:
	Plotter::Image graph_image;
	std::unique_ptr<Fl_RGB_Image> graph_fltk_image;
	std::unique_ptr<Fl_Box> graph_box;
	TextInput<double> step_input;
	TextInput<double> between_input;
	TextInput<double> mouse_x_output;
	TextInput<double> mouse_y_output;
	TextInput<double> mouse_value_output;
	std::unique_ptr<Fl_Button> movement_button;
	std::unique_ptr<Fl_Button> redraw_button;
	std::unique_ptr<Fl_Button> open_button;
	std::unique_ptr<Fl_Button> save_button;
	std::unique_ptr<Fl_Button> export_image_button;
	std::vector<TextInput<std::string>> formula_inputs;
	std::unique_ptr<Fl_Button> carousel_previous_button;
	std::unique_ptr<Fl_Button> carousel_next_button;
	std::unique_ptr<Fl_Box> carousel_index;
	std::unique_ptr<Fl_Check_Button> carousel_active_checkbox;
	ColorInput carousel_color_input;
	std::unique_ptr<Fl_Button> add_system_button;
	std::unique_ptr<Fl_Button> add_levels_button;
	std::unique_ptr<Fl_Button> add_tendency_button;
	std::unique_ptr<Fl_Button> add_div_levels_button;
	std::unique_ptr<Fl_Button> add_div_tendency_button;
	std::unique_ptr<Fl_Button> remove_button;
	std::string carousel_index_text;
	Point zone_center;
	Vector zone_to_corner;
	std::vector<Carousel::Portrait> portraits;
	Carousel carousel;
	std::string current_filename = ".ppb";
	Frame graph_frame;

private:
	static void redrawButtonCallback(Fl_Widget* widget, void* ptr);
	static void openButtonCallback(Fl_Widget* widget, void* ptr);
	static void saveButtonCallback(Fl_Widget* widget, void* ptr);
	static void exportImageButtonCallback(Fl_Widget* widget, void* ptr);
	static void carouselPreviousButtonCallback(Fl_Widget* widget, void* ptr);
	static void carouselNextButtonCallback(Fl_Widget* widget, void* ptr);
	static void addSystemButtonCallback(Fl_Widget* widget, void* ptr);
	static void addLevelsButtonCallback(Fl_Widget* widget, void* ptr);
	static void addTendencyButtonCallback(Fl_Widget* widget, void* ptr);
	static void addDivLevelsButtonCallback(Fl_Widget* widget, void* ptr);
	static void addDivTendencyButtonCallback(Fl_Widget* widget, void* ptr);
	static void removeButtonCallback(Fl_Widget* widget, void* ptr);
	int handle(int event) override;
	void rebuildTracks();
	void saveToCarousel();
	void loadFromCarousel();

public:
	ViewerWindow();
	void redrawImage();
};