#include "tracker.h"

#include "geometry/circle.h"

#include <cmath>
#include <algorithm>
#include <random>


const double Tracker::bounding_step_ratio = 0.3;
const double Tracker::chasing_ratio = 2.0;

Tracker::PointInfo::PointInfo(const Point& point, const Vector& direction,
	double distance): point(point), direction(direction), distance(distance) {}

Tracker::Tracker(const VectorField& field, const Frame& zone, double step,
		double max_between_tracks, double min_between_tracks): field(field), zone(zone),
		step(step), max_between_tracks(max_between_tracks),
		min_between_tracks(min_between_tracks),
		bounding_step(bounding_step_ratio * min_between_tracks),
		chasing_step(chasing_ratio * min_between_tracks),
		base(BASIC_FRAME, max_between_tracks) {
	frameTranslate(this->zone, BASIC_FRAME, this->field);
}

void Tracker::AddToBaseAndCandidates(const Point& point, const Vector& direction) {
	Point left_candidate = point + max_between_tracks * turnedLeft(direction);
	Point right_candidate = point + max_between_tracks * turnedRight(direction);
	if (BASIC_FRAME.isPointInside(left_candidate))
		start_candidates.push(left_candidate);
	if (BASIC_FRAME.isPointInside(right_candidate))
		start_candidates.push(right_candidate);
	base.addPoint(point);
}

void Tracker::goAlongTrack(const Point& start, double speed_sign) {
	Point current = start;
	double travelled_distance = 0;
	double latest_bounding_distance = -2.0 * bounding_step;
	while (true) {
		Vector speed = field.value(current);
		if (speed.length() < EPS)
			break;
		Vector direction = normalized(speed);
		current_track.push_back(current);
		if (travelled_distance - latest_bounding_distance >= bounding_step) {
			latest_bounding_distance = travelled_distance;
			if (travelled_distance <= chasing_step) {
				bounding_tail.emplace_back(current, direction, travelled_distance);
			} else {
				bounding_head.emplace(current, direction, travelled_distance);;
			}
		}
		while (!bounding_head.empty() && travelled_distance - bounding_head.front().distance >
				chasing_step) {
			AddToBaseAndCandidates(bounding_head.front().point, bounding_head.front().direction);
			bounding_head.pop();
		}
		travelled_distance += step;
		current += speed_sign * step * direction;
		if (!BASIC_FRAME.isPointInside(current))
			break;
		if (base.hasNeighbours(current, min_between_tracks)) // !!!
			break;
	}
	while (!bounding_head.empty()) {
		AddToBaseAndCandidates(bounding_head.front().point, bounding_head.front().direction);
		bounding_head.pop();
	}
}

void Tracker::addTrack(const Point& start) {
	current_track.clear();
	bounding_tail.clear();
	bounding_head = std::queue<PointInfo>();
	goAlongTrack(start, -1);
	std::reverse(current_track.begin(), current_track.end());
	goAlongTrack(start, 1);
	tracks.push_back(std::move(current_track));
	for (PointInfo& info : bounding_tail) {
		AddToBaseAndCandidates(info.point, info.direction);
	}
	bounding_tail.clear();
}

void Tracker::addGridOfCandidates() {
	size_t size = std::floor(std::max(10.0, 1.0 / max_between_tracks - 1.0));
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {
			start_candidates.emplace(max_between_tracks * (i + 1), max_between_tracks * (j + 1));
		}
	}
}

std::vector<SegmentedLine> Tracker::getTracks() {
	tracks.clear();
	base.clear();
	start_candidates = std::queue<Point>();
	start_candidates.emplace(0.5, 0.5);
	bool is_grid_tried = false;
	while (!start_candidates.empty() || !is_grid_tried) {
		if (start_candidates.empty()) {
			is_grid_tried = true;
			addGridOfCandidates();
		}
		Point candidate = start_candidates.front();
		start_candidates.pop();
		if (!base.hasNeighbours(candidate, max_between_tracks - EPS))
			addTrack(candidate);
	}
	frameTranslate(BASIC_FRAME, zone, tracks);
	return std::move(tracks);
}