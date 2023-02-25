#include "application.h"


Application::Application() {
	viewer_window = std::make_unique<ViewerWindow>();
}

void Application::run(int argc, char* argv[]) {
	viewer_window->show(argc, argv);
  	Fl::run();
}