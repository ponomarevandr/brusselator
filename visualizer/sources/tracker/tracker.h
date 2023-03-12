#pragma once

#include "geometry/basics.h"
#include "geometry/vector_field.h"
#include "geometry/frame.h"
#include "tracker/neighbours_base.h"

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
	static const double bounding_step_ratio;
	static const double chasing_ratio;
	VectorField field;
	Frame zone;
	double step;
	double max_between_tracks;
	double min_between_tracks;
	double bounding_step;
	double chasing_step;
	std::vector<SegmentedLine> tracks;
	NeighboursBase base;
	std::queue<Point> start_candidates;
	std::vector<Point> current_track;
	std::vector<PointInfo> bounding_tail;
	std::queue<PointInfo> bounding_head;

private:
	void AddToBaseAndCandidates(const Point& point, const Vector& direction);
	void goAlongTrack(const Point& start, double speed_sign);
	void addTrack(const Point& start);
	void addGridOfCandidates();

public:
	Tracker(const VectorField& field, const Frame& zone, double step = 0.001,
		double max_between_tracks = 0.05, double min_between_tracks = 0.025);
	std::vector<SegmentedLine> getTracks();
};