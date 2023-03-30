#include "viewer_window.h"

#include <cstdlib>

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_File_Chooser.H>


void ViewerWindow::redrawButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
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

void ViewerWindow::carouselPreviousButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.toPrevious();
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
}

void ViewerWindow::carouselNextButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.toNext();
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
}

void ViewerWindow::addSystemButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.addElement(Carousel::ElementType::SYSTEM);
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
}

void ViewerWindow::addLevelsButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.addElement(Carousel::ElementType::LEVELS);
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
}

void ViewerWindow::addTendencyButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.addElement(Carousel::ElementType::TENDENCY);
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
}

void ViewerWindow::addDivLevelsButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.addElement(Carousel::ElementType::DIV_LEVELS);
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
}

void ViewerWindow::addDivTendencyButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.addElement(Carousel::ElementType::DIV_TENDENCY);
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
}

void ViewerWindow::removeButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.removeElement();
	static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
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
	if (!step_input.isValid() || !between_input.isValid()) {
		fl_message("Некорректные настройки!");
		return;
	}
	if (!carousel.isValid()) {
		fl_message("Некорректные формулы!");
		return;
	}
	double min_between_tracks = 2.0 / 3.0 * between_input.getValue();
	portraits = carousel.getPortraits(Frame(zone_center - zone_to_corner,
		zone_center + zone_to_corner), step_input.getValue(), 2.0 * min_between_tracks,
		min_between_tracks);
}

void ViewerWindow::saveToCarousel() {
	for (size_t i = 0; i < carousel.getFormulasNumber(); ++i) {
		carousel.setFormulaSymbols(i, formula_inputs[i].getValue());
	}
	carousel.setColor(carousel_color_input.getColor());
}

void ViewerWindow::loadFromCarousel() {
	std::vector<std::string> labels = carousel.getLabels();
	std::vector<std::string> symbols = carousel.getFormulasSymbols();
	for (size_t i = 0; i < labels.size(); ++i) {
		formula_inputs[i].setLabel(labels[i]);
		formula_inputs[i].setValue(symbols[i]);
		formula_inputs[i].show();
	}
	for (size_t i = labels.size(); i < 3; ++i) {
		formula_inputs[i].hide();
	}
	carousel_index_text = std::to_string(carousel.getIndex() + 1);
	carousel_index->label(carousel_index_text.c_str());
	carousel_color_input.setColor(carousel.getColor());
}

ViewerWindow::ViewerWindow():
		Fl_Double_Window(1000, 700, "Построитель фазового портрета"),
		zone_center(0.0, 0.0), zone_to_corner(3.0, 2.0) {
	graph_box = std::make_unique<Fl_Box>(10, 22, 800, 576);
	graph_box->box(FL_UP_BOX);

	step_input = TextInput<double>(820, 10, 170, 30, 85, "шаг");
	step_input.setValue(5e-4);
	between_input = TextInput<double>(820, 40, 170, 30, 85, "отступ");

	between_input.setValue(0.03);
	movement_button = std::make_unique<Fl_Button>(820, 320, 170, 35, "Движение");
	redraw_button = std::make_unique<Fl_Button>(820, 360, 170, 35, "Перестроить");
	redraw_button->callback(ViewerWindow::redrawButtonCallback, this);
	save_button = std::make_unique<Fl_Button>(820, 400, 170, 35, "Сохранить");
	save_button->callback(ViewerWindow::saveButtonCallback, this);
	add_system_button = std::make_unique<Fl_Button>(820, 455, 170, 35, "Cистема");
	add_system_button->callback(ViewerWindow::addSystemButtonCallback, this);
	add_levels_button = std::make_unique<Fl_Button>(820, 495, 170, 35, "Уровни функции");
	add_levels_button->callback(ViewerWindow::addLevelsButtonCallback, this);
	add_tendency_button = std::make_unique<Fl_Button>(820, 535, 170, 35, "Функция к 0");
	add_tendency_button->callback(ViewerWindow::addTendencyButtonCallback, this);
	add_div_levels_button = std::make_unique<Fl_Button>(820, 575, 170, 35, "Уровни дивергенции");
	add_div_levels_button->callback(ViewerWindow::addDivLevelsButtonCallback, this);
	add_div_tendency_button = std::make_unique<Fl_Button>(820, 615, 170, 35, "Дивергенция к 0");
	add_div_tendency_button->callback(ViewerWindow::addDivTendencyButtonCallback, this);
	remove_button = std::make_unique<Fl_Button>(820, 655, 170, 35, "Удалить");
	remove_button->callback(ViewerWindow::removeButtonCallback, this);

	carousel_previous_button = std::make_unique<Fl_Button>(10, 605, 30, 50, "<");
	carousel_previous_button->callback(ViewerWindow::carouselPreviousButtonCallback, this);
	carousel_index = std::make_unique<Fl_Box>(FL_NO_BOX, 40, 605, 40, 50, "");
	carousel_next_button = std::make_unique<Fl_Button>(80, 605, 30, 50, ">");
	carousel_next_button->callback(ViewerWindow::carouselNextButtonCallback, this);
	carousel_color_input = ColorInput(10, 660, 100, 30, "");
	formula_inputs.emplace_back(125, 605, 685, 25, 70);
	formula_inputs.emplace_back(125, 635, 685, 25, 70);
	formula_inputs.emplace_back(125, 665, 685, 25, 70);
	loadFromCarousel();
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