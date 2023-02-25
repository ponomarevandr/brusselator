#include "viewer_window.h"

#include "geometry/basics.h"
#include "plotter/plotter.h"

#include <algorithm>

#include <FL/Fl_PNG_Image.H>


void ViewerWindow::redrawButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->redrawImage();
}

ViewerWindow::ViewerWindow():
		Fl_Double_Window(1000, 700, "Просмотр фазового портрета") {
	button_redraw = std::make_unique<Fl_Button>(820, 550, 170, 48, "Перестроить");
	button_redraw->callback(ViewerWindow::redrawButtonCallback, this);
	graph_box = std::make_unique<Fl_Box>(10, 22, 800, 576);
	graph_box->box(FL_UP_BOX);
}

void ViewerWindow::redrawImage() {
	std::vector<SegmentedLine> lines;
	SegmentedLine line;
	line.push_back(Point(0, 0));
	line.push_back(Point(1, 1));
	line.push_back(Point(1, 2));
	line.push_back(Point(0, 1.5));
	lines.push_back(std::move(line));
	line = SegmentedLine();
	line.push_back(Point(0, 0));
	line.push_back(Point(2, 1));
	line.push_back(Point(2, -2));
	lines.push_back(std::move(line));
	std::string filename = Plotter::plot(800, 600, lines);

	graph = std::make_unique<Fl_PNG_Image>(filename.c_str());
	graph_box->image(graph.get());
	redraw();
}