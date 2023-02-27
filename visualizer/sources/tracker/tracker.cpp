#include "tracker.h"

#include <cmath>
#include <algorithm>


const Frame Tracker::basic = Frame(1, 1);
const double Tracker::between_vertices = 0.005;

Tracker::Tracker(const VectorField& field, double step, const Frame& zone):
		field(field), step(step), zone(zone) {
	frameTranslate(this->zone, basic, this->field);
}

void Tracker::thinOutTrack(SegmentedLine& track) {
	if (track.empty() || track.size() == 1)
		return;
	SegmentedLine result;
	result.push_back(track[0]);
	double passed_distance = 0;
	for (size_t i = 1; i + 1 < track.size(); ++i) {
		passed_distance += distance(track[i - 1], track[i]);
		if (passed_distance >= between_vertices) {
			result.push_back(track[i]);
			passed_distance = 0;
		}
	}
	result.push_back(track.back());
	track = std::move(result);
}

SegmentedLine Tracker::getTrack(const Point& start) {
	SegmentedLine result;
	Point current = start;
	for (size_t i = 0; i < 5 * std::round(1 / step); ++i) {
		result.push_back(current);
		Vector v = field.value(current);
		if (v.length() < 0.000001)
			break;
		current -= step * normalized(v);
		if (!basic.isPointInside(current))
			break;
	}
	current = start;
	std::reverse(result.begin(), result.end());
	for (size_t i = 0; i < 5 * std::round(1 / step); ++i) {
		result.push_back(current);
		Vector v = field.value(current);
		if (v.length() < 0.000001)
			break;
		current += step * normalized(v);
		if (!basic.isPointInside(current))
			break;
	}
	thinOutTrack(result);
	return result;
}

std::vector<SegmentedLine> Tracker::getAllTracks() {
	std::vector<SegmentedLine> tracks;
	tracks.push_back(getTrack(Point(0.75, 0.75)));
	frameTranslate(basic, zone, tracks);
	return tracks;
}