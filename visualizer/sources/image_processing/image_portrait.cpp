#include "image_portrait.h"

#include "geometry/vector_field.h"
#include "tracker/tracker.h"
#include "tracker/visual_preparator.h"

#include <cmath>


ImagePortrait::ImagePortrait(ImageLoader& image): image(image), direction(image) {}

void ImagePortrait::prepare() {
	direction.calculate();
}

ImagePortrait::Portrait ImagePortrait::getPortrait(const Frame& zone, double step,
		double max_between_tracks, double min_between_tracks) const {
	VectorField field([this](double x, double y) -> double {
		int whole_x = std::round(x);
		int whole_y = image.getHeight() - 1 - std::round(y);
		if (whole_x < 0 || whole_x >= this->image.getWidth() || whole_y < 0 ||
				whole_y >= this->image.getHeight()) {
			return 0;
		}
		return -this->direction.getDirection(whole_x, whole_y).x;
	}, [this](double x, double y) -> double {
		int whole_x = std::round(x);
		int whole_y = image.getHeight() - 1 - std::round(y);
		if (whole_x < 0 || whole_x >= this->image.getWidth() || whole_y < 0 ||
				whole_y >= this->image.getHeight()) {
			return 0;
		}
		return this->direction.getDirection(whole_x, whole_y).y;
	});
	Tracker tracker(field, zone, step, max_between_tracks, min_between_tracks);
	std::vector<SegmentedLine> tracks = tracker.getTracks();
	VisualPreparator preparator(tracks, zone, true);
	preparator.prepareTracks();
	return std::make_pair(tracks, Plotter::Color::RED);
}