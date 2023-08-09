#include "viewer/application.h"

#include "image_processing/image_loader.h"
#include "image_processing/image_direction.h"

#include <iostream>


int main(int argc, char* argv[]) {
	Application application;
	application.run(argc, argv);
	/*ImageLoader image("liz.png");
	image.load();
	ImageDirection direction(image, 3);
	direction.calculate();
	while (true) {
		size_t x, y;
		std::cin >> x >> y;
		direction.getDirection(x, y).print();
	}*/
}