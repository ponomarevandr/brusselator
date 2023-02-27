#include "geometry/basics.h"
#include "geometry/frame.h"
#include "geometry/vector_field.h"
#include "viewer/application.h"

#include <iostream>

double vx(double x, double y) {
	return x;
}

double vy(double x, double y) {
	return y;
}

int main(int argc, char* argv[]) {
	//Application application;
	//application.run(argc, argv);
	Frame source(1, 1);
	Frame target(Point(10, 10), 2, 1);
	target.getBottomLeft().print();
	target.getTopRight().print();
	VectorField v(vx, vy);
	frameTranslate(source, target, v);
	while (true) {
		double x;
		double y;
		std::cin >> x >> y;
		v.value(Point(x, y)).print();
		v.direction(Point(x, y)).print();
	}
}