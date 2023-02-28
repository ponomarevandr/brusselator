#pragma once

#include "geometry/basics.h"
#include "geometry/vector_field.h"
#include "geometry/frame.h"
#include "tracker/neighbours_base.h"

#include <vector>
#include <queue>


class Tracker {
private:
	static const Frame basic;
	static const double bounding_step_ratio;
	static const double chasing_ratio;
	VectorField field;
	Frame zone;
	double step;
	double plot_step;
	double max_between_tracks;
	double min_between_tracks;
	double bounding_step;
	std::vector<SegmentedLine> tracks;
	NeighboursBase base;
	std::queue<Point> start_candidates;
	std::vector<Point> current_track;
	std::vector<Point> tail;
	std::queue<Point> to_add_to_base;
	std::queue<double> to_add_to_base_distance;

private:
	void addPointAndTakeIntersections(const Point&);
	void goAlongTrack(const Point& start, double direction);
	void addTrack(const Point& start);

public:
	Tracker(const VectorField& field, const Frame& zone, double step = 0.0005, double plot_step = 0.01,
		double max_between_tracks = 0.05, double min_between_tracks = 0.02);
	std::vector<SegmentedLine> getTracks();
};