#include "viewer/application.h"

#include "image_processing/image_loader.h"

#include <iostream>


int main(int argc, char* argv[]) {
	//Application application;
	//application.run(argc, argv);
	ImageLoader image("liz.png");
	image.load();
	std::cout << static_cast<size_t>(image.getPixelR(639, 639)) << "\n";
	std::cout << static_cast<size_t>(image.getPixelG(639, 639)) << "\n";
	std::cout << static_cast<size_t>(image.getPixelB(639, 639)) << "\n";
	std::cout << image.getPixelV(639, 639) << "\n";
}