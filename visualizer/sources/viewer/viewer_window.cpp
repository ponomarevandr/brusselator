#include "viewer_window.h"

#include "geometry/basics.h"

#include <cstdlib>
#include <algorithm>

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_File_Chooser.H>


void ViewerWindow::redrawButtonCallback(Fl_Widget* widget, void* ptr) {
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

ViewerWindow::ViewerWindow():
		Fl_Double_Window(1000, 700, "Построитель фазового портрета") {
	graph_box = std::make_unique<Fl_Box>(10, 22, 800, 576);
	graph_box->box(FL_UP_BOX);
	redraw_button = std::make_unique<Fl_Button>(820, 488, 170, 50, "Перестроить");
	redraw_button->callback(ViewerWindow::redrawButtonCallback, this);
	save_button = std::make_unique<Fl_Button>(820, 548, 170, 50, "Сохранить");
	save_button->callback(ViewerWindow::saveButtonCallback, this);
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
	graph_image = Plotter::plot(800, 600, lines);

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