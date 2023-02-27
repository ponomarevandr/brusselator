#pragma once

#include "geometry/basics.h"
#include "geometry/vector_field.h"
#include "geometry/frame.h"

#include <vector>


class Tracker {
private:
	static const Frame basic;
	static const double between_vertices;
	VectorField field;
	double step;
	Frame zone;

private:
	void thinOutTrack(SegmentedLine& track);
	SegmentedLine getTrack(const Point& start);

public:
	Tracker(const VectorField& field, double step, const Frame& zone);
	std::vector<SegmentedLine> getAllTracks();
};