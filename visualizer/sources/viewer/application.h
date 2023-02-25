#pragma once

#include "viewer/viewer_window.h"

#include <memory>


class Application {
private:
	std::unique_ptr<ViewerWindow> viewer_window;

public:
	Application();
	void run(int argc, char* argv[]);
};