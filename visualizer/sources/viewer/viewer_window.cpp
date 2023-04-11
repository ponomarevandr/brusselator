#include "viewer_window.h"

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_File_Chooser.H>

#include <iostream>


void ViewerWindow::redrawButtonCallback(Fl_Widget* widget, void* ptr) {
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->rebuildTracks();
	static_cast<ViewerWindow*>(ptr)->redrawImage();
}

void ViewerWindow::openButtonCallback(Fl_Widget* widget, void* ptr) {
	char* filename = fl_file_chooser("Открыть рабочее пространство", "*.ppb", ".ppb", 0);
	if (!filename)
		return;
	if (!static_cast<ViewerWindow*>(ptr)->carousel.loadFromFile(filename)) {
		fl_message("Файл отсутствует!");
	} else {
		static_cast<ViewerWindow*>(ptr)->current_filename = filename;
		static_cast<ViewerWindow*>(ptr)->loadFromCarousel();
	}
}

void ViewerWindow::saveButtonCallback(Fl_Widget* widget, void* ptr) {
	char* filename = fl_file_chooser("Сохранить рабочее пространство", "*.ppb",
		static_cast<ViewerWindow*>(ptr)->current_filename.c_str(), 0);
	if (!filename)
		return;
	static_cast<ViewerWindow*>(ptr)->saveToCarousel();
	static_cast<ViewerWindow*>(ptr)->carousel.saveToFile(filename);
}

void ViewerWindow::exportImageButtonCallback(Fl_Widget* widget, void* ptr) {
	char* filename = fl_file_chooser("Экспорт изображения", "*.png", ".png", 0);
	if (!filename)
		return;
	static_cast<ViewerWindow*>(ptr)->graph_image.save(filename);
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
			is_image_fresh = false;
			break;
		case 'a':
			zone_center -= Vector(zone_to_corner.x * 0.1, 0);
			is_image_fresh = false;
			break;
		case 's':
			zone_center -= Vector(0, zone_to_corner.y * 0.1);
			is_image_fresh = false;
			break;
		case 'd':
			zone_center += Vector(zone_to_corner.x * 0.1, 0);
			is_image_fresh = false;
			break;
		case '=':
			zone_to_corner /= 1.1;
			is_image_fresh = false;
			break;
		case '-':
			zone_to_corner *= 1.1;
			is_image_fresh = false;
			break;
		}
		redrawImage();
	}
	if ((event == FL_KEYDOWN || event == FL_MOVE || event == FL_DRAG) && is_image_fresh) {
		Point mouse(Fl::event_x(), Fl::event_y());
		if (graph_frame.isPointInside(mouse)) {
			Frame working_zone(zone_center - zone_to_corner, zone_center + zone_to_corner);
			frameTranslate(graph_frame, BASIC_FRAME, mouse);
			mouse.y = 1.0 - mouse.y;
			frameTranslate(BASIC_FRAME, working_zone, mouse);
			mouse_x_output.setValue(mouse.x);
			mouse_y_output.setValue(mouse.y);
			mouse_value_output.setValue(carousel.getFunctionValue(mouse));
		}
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
	is_image_fresh = true;
}

void ViewerWindow::saveToCarousel() {
	for (size_t i = 0; i < carousel.getFormulasNumber(); ++i) {
		carousel.setFormulaSymbols(i, formula_inputs[i].getValue());
	}
	carousel.setColor(carousel_color_input.getColor());
	carousel.setIsActive(carousel_active_checkbox->value());
	checkCoordinatesMatrix();
	Matrix22 coordinates_matrix;
	coordinates_matrix.values[0][0] = coordinates_matrix_inputs[0].getValue();
	coordinates_matrix.values[0][1] = coordinates_matrix_inputs[1].getValue();
	coordinates_matrix.values[1][0] = coordinates_matrix_inputs[2].getValue();
	coordinates_matrix.values[1][1] = coordinates_matrix_inputs[3].getValue();
	carousel.setCoordinatesMatrix(coordinates_matrix);
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
	carousel_active_checkbox->value(carousel.getIsActive());
	Matrix22 coordinates_matrix = carousel.getCoordinatesMatrix();
	coordinates_matrix_inputs[0].setValue(coordinates_matrix.values[0][0]);
	coordinates_matrix_inputs[1].setValue(coordinates_matrix.values[0][1]);
	coordinates_matrix_inputs[2].setValue(coordinates_matrix.values[1][0]);
	coordinates_matrix_inputs[3].setValue(coordinates_matrix.values[1][1]);
}

void ViewerWindow::checkCoordinatesMatrix() {
	bool found = false;
	for (size_t i = 0; i < 4; ++i) {
		if (coordinates_matrix_inputs[i].isValid())
			continue;
		coordinates_matrix_inputs[i].setValue(0);
		found = true;
	}
	if (found)
		fl_message("Некорректная матрица координат! Занулено");
}

ViewerWindow::ViewerWindow():
		Fl_Double_Window(1000, 700, "Построитель фазового портрета"),
		zone_center(0.0, 0.0), zone_to_corner(3.0, 2.0), is_image_fresh(false) {
	graph_box = std::make_unique<Fl_Box>(10, 22, 800, 576);
	graph_box->box(FL_UP_BOX);
	graph_frame = Frame(Point(121, 104), Point(700, 519));

	step_input = TextInput<double>(810, 10, 185, 30, 85, "шаг");
	step_input.setValue(5e-4);
	between_input = TextInput<double>(810, 40, 185, 30, 85, "отступ");
	mouse_x_output = TextInput<double>(810, 90, 185, 30, 85, "x мыши");
	mouse_x_output.setIsActive(false);
	mouse_y_output = TextInput<double>(810, 120, 185, 30, 85, "y мыши");
	mouse_y_output.setIsActive(false);
	mouse_value_output = TextInput<double>(810, 150, 185, 30, 85, "значение");
	mouse_value_output.setIsActive(false);

	coordinates_matrix_label = std::make_unique<Fl_Box>(FL_NO_BOX, 820, 200, 170, 30,
		"замена координат");
	coordinates_matrix_inputs.emplace_back(820, 230, 80, 30, 0);
	coordinates_matrix_inputs.emplace_back(910, 230, 80, 30, 0);
	coordinates_matrix_inputs.emplace_back(820, 260, 80, 30, 0);
	coordinates_matrix_inputs.emplace_back(910, 260, 80, 30, 0);

	between_input.setValue(0.03);
	movement_button = std::make_unique<Fl_Button>(820, 330, 170, 30, "Движение");
	redraw_button = std::make_unique<Fl_Button>(820, 365, 170, 30, "Перестроить");
	redraw_button->callback(ViewerWindow::redrawButtonCallback, this);
	open_button = std::make_unique<Fl_Button>(820, 400, 75, 30, "Открыть");
	open_button->callback(ViewerWindow::openButtonCallback, this);
	save_button = std::make_unique<Fl_Button>(900, 400, 90, 30, "Сохранить");
	save_button->callback(ViewerWindow::saveButtonCallback, this);
	export_image_button = std::make_unique<Fl_Button>(820, 435, 170, 30, "Экспорт изображения");
	export_image_button->callback(ViewerWindow::exportImageButtonCallback, this);
	add_system_button = std::make_unique<Fl_Button>(820, 485, 170, 30, "Cистема");
	add_system_button->callback(ViewerWindow::addSystemButtonCallback, this);
	add_levels_button = std::make_unique<Fl_Button>(820, 520, 170, 30, "Уровни функции");
	add_levels_button->callback(ViewerWindow::addLevelsButtonCallback, this);
	add_tendency_button = std::make_unique<Fl_Button>(820, 555, 170, 30, "Функция к 0");
	add_tendency_button->callback(ViewerWindow::addTendencyButtonCallback, this);
	add_div_levels_button = std::make_unique<Fl_Button>(820, 590, 170, 30, "Уровни дивергенции");
	add_div_levels_button->callback(ViewerWindow::addDivLevelsButtonCallback, this);
	add_div_tendency_button = std::make_unique<Fl_Button>(820, 625, 170, 30, "Дивергенция к 0");
	add_div_tendency_button->callback(ViewerWindow::addDivTendencyButtonCallback, this);
	remove_button = std::make_unique<Fl_Button>(820, 660, 170, 30, "Удалить");
	remove_button->callback(ViewerWindow::removeButtonCallback, this);

	carousel_previous_button = std::make_unique<Fl_Button>(10, 605, 30, 50, "<");
	carousel_previous_button->callback(ViewerWindow::carouselPreviousButtonCallback, this);
	carousel_index = std::make_unique<Fl_Box>(FL_NO_BOX, 40, 605, 40, 50, "");
	carousel_next_button = std::make_unique<Fl_Button>(80, 605, 30, 50, ">");
	carousel_next_button->callback(ViewerWindow::carouselNextButtonCallback, this);
	carousel_color_input = ColorInput(10, 660, 60, 30, "");
	carousel_active_checkbox = std::make_unique<Fl_Check_Button>(80, 660, 30, 30);
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