#pragma once

#include "geometry/basics.h"
#include "geometry/frame.h"

#include <vector>


class VisualPreparator {
private:
	std::vector<SegmentedLine>& tracks;
	Frame zone;
	bool with_arrows;
	double plot_step;
	double arrow_step;
	double arrow_size;

public:
	VisualPreparator(std::vector<SegmentedLine>& tracks, const Frame& zone, bool with_arrows,
		double plot_step = 0.01, double arrow_step = 0.2, double arrow_size = 0.015);
	void prepareTracks();
};