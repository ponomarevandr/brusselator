#include "tracker.h"

#include "geometry/circle.h"

#include <cmath>
#include <algorithm>
#include <random>


const Frame Tracker::basic = Frame(1, 1);
const double Tracker::bounding_step_ratio = 0.3;
const double Tracker::chasing_ratio = 2.0;

Tracker::Tracker(const VectorField& field, const Frame& zone, double step, double plot_step,
		double max_between_tracks, double min_between_tracks): field(field), zone(zone),
		step(step), plot_step(plot_step), max_between_tracks(max_between_tracks),
		min_between_tracks(min_between_tracks),
		bounding_step(min_between_tracks * bounding_step_ratio),
		base(basic, 2.0 * max_between_tracks) {
	frameTranslate(this->zone, basic, this->field);
}

void Tracker::addPointAndTakeIntersections(const Point& point) {
	std::vector<Point> neighbours = base.getNeighbours(point, 2.0 * max_between_tracks);
	Circle circle(point, max_between_tracks);
	for (const Point& neighbour : neighbours) {
		Circle other(neighbour, max_between_tracks);
		std::vector<Point> intersections = intersectionPoints(circle, other);
		for (const Point& candidate : intersections) {
			if (basic.isPointInside(candidate))
				start_candidates.push(candidate);
		}
	}
	base.addPoint(point);
}


void Tracker::goAlongTrack(const Point& start, double direction) {
	Point current = start;
	current_track.push_back(current);
	tail.push_back(current);
	double travelled_distance = 0;
	double latest_plot_distance = 0;
	double latest_bounding_distane = 0;
	while (true) {
		Vector speed = field.value(current);
		if (speed.length() < EPS)
			break;
		current += direction * step * normalized(speed);
		travelled_distance += step;
		if (!basic.isPointInside(current))
			break;
		if (base.hasNeighbours(current, min_between_tracks))
			break;
		if (travelled_distance - latest_plot_distance >= plot_step) {
			latest_plot_distance = travelled_distance;
			current_track.push_back(current);
		}
		if (travelled_distance - latest_bounding_distane >= bounding_step) {
			latest_bounding_distane = travelled_distance;
			if (travelled_distance <= chasing_ratio * min_between_tracks) {
				tail.push_back(current);
			} else {
				to_add_to_base.push(current);
				to_add_to_base_distance.push(travelled_distance);
			}
		}
		while (!to_add_to_base.empty() && travelled_distance - to_add_to_base_distance.front() >
				chasing_ratio * min_between_tracks) {
			addPointAndTakeIntersections(to_add_to_base.front());
			to_add_to_base.pop();
			to_add_to_base_distance.pop();
		}
	}
	while (!to_add_to_base.empty()) {
		addPointAndTakeIntersections(to_add_to_base.front());
		to_add_to_base.pop();
		to_add_to_base_distance.pop();
	}
}

void Tracker::addTrack(const Point& start) {
	current_track.clear();
	tail.clear();
	to_add_to_base = std::queue<Point>();
	to_add_to_base_distance = std::queue<double>();
	goAlongTrack(start, -1);
	std::reverse(current_track.begin(), current_track.end());
	goAlongTrack(start, 1);
	tracks.push_back(std::move(current_track));
	for (const Point& point : tail) {
		base.addPoint(point);
	}
	tail.clear();
}

std::vector<SegmentedLine> Tracker::getTracks() {
	tracks.clear();
	base.clear();
	start_candidates = std::queue<Point>();
	start_candidates.push(Point(0.5, 0.5));
	start_candidates.push(Point(0.5 + max_between_tracks, 0.5));
	while (!start_candidates.empty()) {
		Point candidate = start_candidates.front();
		start_candidates.pop();
		if (!base.hasNeighbours(candidate, max_between_tracks - EPS))
			addTrack(candidate);
	}
	frameTranslate(basic, zone, tracks);
	return std::move(tracks);
}