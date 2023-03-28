#include "viewer_window.h"

#include <cstdlib>

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
	std::free(filename);
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

void ViewerWindow::rebuildTracks() {
	carousel.setFormulaSymbols(0, formula_inputs[0]->value());
	carousel.setFormulaSymbols(1, formula_inputs[1]->value());
	if (!carousel.isValid()) {
		fl_message("Некорректные формулы!");
		return;
	}
	portraits = carousel.getPortraits(Frame(zone_center - zone_to_corner,
		zone_center + zone_to_corner));
}

ViewerWindow::ViewerWindow():
		Fl_Double_Window(1000, 700, "Построитель фазового портрета"),
		zone_center(0.0, 0.0), zone_to_corner(3.0, 2.0) {
	graph_box = std::make_unique<Fl_Box>(10, 22, 800, 576);
	graph_box->box(FL_UP_BOX);
	movement_button = std::make_unique<Fl_Button>(820, 428, 170, 50, "Движение");
	redraw_button = std::make_unique<Fl_Button>(820, 488, 170, 50, "Перестроить");
	redraw_button->callback(ViewerWindow::redrawButtonCallback, this);
	save_button = std::make_unique<Fl_Button>(820, 548, 170, 50, "Сохранить");
	save_button->callback(ViewerWindow::saveButtonCallback, this);
	formula_labels.push_back(std::make_unique<Fl_Box>(FL_NO_BOX, 10, 605, 40, 25, "x' ="));
	formula_labels.push_back(std::make_unique<Fl_Box>(FL_NO_BOX, 10, 635, 40, 25, "y' ="));
	formula_labels.push_back(std::make_unique<Fl_Box>(FL_NO_BOX, 10, 665, 40, 25, "---"));
	formula_inputs.push_back(std::make_unique<Fl_Input>(60, 605, 740, 25));
	formula_inputs.push_back(std::make_unique<Fl_Input>(60, 635, 740, 25));
	formula_inputs.push_back(std::make_unique<Fl_Input>(60, 665, 740, 25));
	redrawImage();
}

void ViewerWindow::redrawImage() {
	Plotter plotter(800, 600);
	for (Carousel::Portrait& portrait : portraits) {
		plotter.addPortrait(portrait.first, portrait.second);
	}
	Vector to_other_corner = Vector(zone_to_corner.x, -zone_to_corner.y);
	std::vector<SegmentedLine> view_frame(1);
	view_frame[0] = {
		zone_center - zone_to_corner,
		zone_center - to_other_corner,
		zone_center + zone_to_corner,
		zone_center + to_other_corner,
		zone_center - zone_to_corner
	};
	plotter.addPortrait(view_frame, Plotter::Color::BLACK);
	graph_image = plotter.getImage();
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