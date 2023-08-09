#pragma once

#include "geometry/basics.h"
#include "geometry/frame.h"
#include "plotter/plotter.h"
#include "image_processing/image_loader.h"
#include "image_processing/image_direction.h"


class ImagePortrait {
public:
	using Portrait = std::pair<std::vector<SegmentedLine>, Plotter::Color>;

private:
	ImageLoader& image;
	ImageDirection direction;

public:
	explicit ImagePortrait(ImageLoader& image);
	void prepare();
	Portrait getPortrait(const Frame& zone, double step, double max_between_tracks,
		double min_between_tracks) const;
};