#include "visual_preparator.h"

#include <algorithm>


VisualPreparator::VisualPreparator(std::vector<SegmentedLine>& tracks, const Frame& zone,
	bool with_arrows, double plot_step, double arrow_step, double arrow_size): tracks(tracks),
	zone(zone), with_arrows(with_arrows), plot_step(plot_step), arrow_step(arrow_step),
	arrow_size(arrow_size) {}

void VisualPreparator::prepareTracks() {
	frameTranslate(zone, BASIC_FRAME, tracks);
	for (SegmentedLine& track : tracks) {
		if (track.size() <= 2)
			continue;
		SegmentedLine transformed;
		transformed.push_back(track[0]);
		double travelled_distance = 0;
		double latest_plot_distance = 0;
		double latest_arrow_distance = -arrow_step / 2.0;
		for (size_t i = 1; i + 1 < track.size(); ++i) {
			travelled_distance += distanceBetween(track[i - 1], track[i]);
			if (travelled_distance - latest_plot_distance >= plot_step) {
				latest_plot_distance = travelled_distance;
				transformed.push_back(track[i]);
				if (with_arrows && travelled_distance - latest_arrow_distance >= arrow_step) {
					latest_arrow_distance = travelled_distance;
					Point current = transformed.back();
					Point previous = transformed[transformed.size() - 2];
					Vector direction = normalized(current - previous);
					Point left_wing = current + arrow_size * turned(direction, 5.0 * PI / 4.0);
					Point right_wing = current + arrow_size * turned(direction, -5.0 * PI / 4.0);
					transformed.push_back(left_wing);
					transformed.push_back(current);
					transformed.push_back(right_wing);
					transformed.push_back(current);
				}
			}
		}
		transformed.push_back(track.back());
		std::swap(track, transformed);
	}
	frameTranslate(BASIC_FRAME, zone, tracks);
}