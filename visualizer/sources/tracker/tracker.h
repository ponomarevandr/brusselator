#pragma once

#include "geometry/basics.h"
#include "geometry/vector_field.h"
#include "geometry/frame.h"
#include "tracker/neighbours_base.h"

#include <iostream>
#include <vector>
#include <queue>


class Tracker {
private:
	struct PointInfo {
		Point point;
		Vector direction;
		double distance;

		PointInfo() = default;
		PointInfo(const Point& point, const Vector& vector, double distance = 0);
	};

private:
	static const double BOUNDING_STEP_RATIO;
	static const double CHASING_RATIO;
	VectorField field;
	Frame zone;
	double step;
	double max_between_tracks;
	double min_between_tracks;
	double bounding_step;
	double chasing_distance;
	std::vector<SegmentedLine> tracks;
	NeighboursBase max_distance_base;
	NeighboursBase min_distance_base;
	std::queue<Point> start_candidates;
	std::vector<Point> current_track;
	std::vector<PointInfo> bounding_tail;
	std::queue<PointInfo> bounding_head;
	size_t millis_elapsed;
	size_t tracks_generated;
	size_t points_generated;

private:
	void AddToBaseAndCandidates(const Point& point, const Vector& direction);
	void goAlongTrack(const Point& start, double speed_sign);
	void addTrack(const Point& start);
	void addGridOfCandidates();

public:
	Tracker(const VectorField& field, const Frame& zone, double step = 0.0005,
		double max_between_tracks = 0.04, double min_between_tracks = 0.02);
	std::vector<SegmentedLine> getTracks();
	void printReport(std::ostream&);
};