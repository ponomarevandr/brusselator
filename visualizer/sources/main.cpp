#include "geometry/basics.h"
#include "plotter/plotter.h"


int main() {
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
	Plotter::plot(800, 600, lines);
}