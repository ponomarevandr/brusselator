#include "viewer_window.h"

#include "geometry/vector_field.h"
#include "tracker/tracker.h"
#include "tracker/visual_preparator.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_File_Chooser.H>


void ViewerWindow::redrawButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->rebuildTracks();
	static_cast<ViewerWindow*>(ptr)->redrawImage();
}

void ViewerWindow::saveButtonCallback(Fl_Widget* widget, void* ptr) {
	char* filename = fl_file_chooser("Введите имя файла", ".png", ".png", 0);
	if (!filename)
		return;
	if (!static_cast<ViewerWindow*>(ptr)->saveImage(filename))
		fl_message("Изображение отсутствует!");
	free(filename);
}

int ViewerWindow::handle(int event) {
	if (event == FL_KEYDOWN) {
		switch (static_cast<char>(Fl::event_key())) {
		case 'w':
			zone_center += Vector(0, zone_to_corner.y * 0.1);
			break;
		case 'a':
			zone_center -= Vector(zone_to_corner.x * 0.1, 0);
			break;
		case 's':
			zone_center -= Vector(0, zone_to_corner.y * 0.1);
			break;
		case 'd':
			zone_center += Vector(zone_to_corner.x * 0.1, 0);
			break;
		case '=':
			zone_to_corner /= 1.1;
			break;
		case '-':
			zone_to_corner *= 1.1;
			break;
		case 'r':
			rebuildTracks();
			break;
		default:
			return Fl_Double_Window::handle(event);
		}
		redrawImage();
		return 0;
	}
	return Fl_Double_Window::handle(event);
}

double v_a = 3.0;
double v_b = 3.0;

double vx(double x, double y) {
	return v_a - (v_b + 1.0) * x + x * x * y;
}

double vy(double x, double y) {
	return v_b * x - x * x * y;
}

void ViewerWindow::rebuildTracks() {
	Frame working_zone(zone_center - zone_to_corner, zone_center + zone_to_corner);
	VectorField field(vx, vy);
	Tracker tracker(field, working_zone);
	tracks = tracker.getTracks();
	//tracker.printReport(std::cout);
	VisualPreparator preparator(tracks, working_zone, true);
	preparator.prepareTracks();
}

ViewerWindow::ViewerWindow():
		Fl_Double_Window(1000, 700, "Построитель фазового портрета"),
		zone_center(1.5, 1.0), zone_to_corner(3.0, 2.0) {
	graph_box = std::make_unique<Fl_Box>(10, 22, 800, 576);
	graph_box->box(FL_UP_BOX);
	redraw_button = std::make_unique<Fl_Button>(820, 488, 170, 50, "Перестроить");
	redraw_button->callback(ViewerWindow::redrawButtonCallback, this);
	save_button = std::make_unique<Fl_Button>(820, 548, 170, 50, "Сохранить");
	save_button->callback(ViewerWindow::saveButtonCallback, this);
}

void ViewerWindow::redrawImage() {
	tracks.emplace_back();
	Vector to_other_corner = Vector(zone_to_corner.x, -zone_to_corner.y);
	tracks.back().push_back(zone_center - zone_to_corner);
	tracks.back().push_back(zone_center - to_other_corner);
	tracks.back().push_back(zone_center + zone_to_corner);
	tracks.back().push_back(zone_center + to_other_corner);
	tracks.back().push_back(zone_center - zone_to_corner);
	graph_image = Plotter::plot(800, 600, tracks);
	tracks.pop_back();

	graph_fltk_image = std::make_unique<Fl_PNG_Image>(nullptr,
		graph_image.getBuffer<unsigned char>(), graph_image.getSize());
	graph_box->image(graph_fltk_image.get());
	redraw();
}

bool ViewerWindow::saveImage(const std::string& filename) const {
	if (!graph_image.isValid())
		return false;
	graph_image.save(filename);
	return true;
}