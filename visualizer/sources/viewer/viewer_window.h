#pragma once

#include "geometry/basics.h"
#include "geometry/frame.h"
#include "plotter/plotter.h"

#include <string>
#include <memory>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>


class ViewerWindow: public Fl_Double_Window {
private:
	Plotter::Image graph_image;
	std::unique_ptr<Fl_RGB_Image> graph_fltk_image;
	std::unique_ptr<Fl_Box> graph_box;
	std::unique_ptr<Fl_Button> redraw_button;
	std::unique_ptr<Fl_Button> save_button;
	Point zone_center;
	Vector zone_to_corner;
	std::vector<SegmentedLine> tracks;

private:
	static void redrawButtonCallback(Fl_Widget* widget, void* ptr);
	static void saveButtonCallback(Fl_Widget* widget, void* ptr);
	int handle(int event) override;
	void rebuildTracks();

public:
	ViewerWindow();
	void redrawImage();
	bool saveImage(const std::string& filename) const;
};