#pragma once

#include <memory>
#include <functional>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>


class ViewerWindow: public Fl_Double_Window {
private:
	std::unique_ptr<Fl_RGB_Image> graph;
	std::unique_ptr<Fl_Box> graph_box;
	std::unique_ptr<Fl_Button> button_redraw;

private:
	static void redrawButtonCallback(Fl_Widget* widget, void* ptr);

public:
	ViewerWindow();
	void redrawImage();
};