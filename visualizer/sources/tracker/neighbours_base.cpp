#include "neighbours_base.h"

#include "geometry/circle.h"

#include <cmath>


NeighboursBase::NeighboursBase(const Frame& zone, double max_radius):
		zone(zone), max_radius(max_radius) {
	size_t width = 4 + std::ceil(this->zone.width() / max_radius);
	size_t height = 4 + std::ceil(this->zone.height() / max_radius);
	base.resize(width);
	for (size_t i = 0; i < width; ++i) {
		base[i].resize(height);
	}
}

std::pair<size_t, size_t> NeighboursBase::getSector(const Point& point) const {
	Point moved = point - Vector(zone.getBottomLeft());
	size_t i = static_cast<size_t>(2.0 + std::floor(moved.x / max_radius));
	size_t j = static_cast<size_t>(2.0 + std::floor(moved.y / max_radius));
	return std::make_pair(i, j);
}

bool NeighboursBase::addPoint(const Point& point) {
	if (!zone.isPointInside(point))
		return false;
	std::pair<size_t, size_t> sector = getSector(point);
	base[sector.first][sector.second].push_back(point);
	return true;
}

bool NeighboursBase::hasNeighbours(const Point& point, double radius) const {
	if (!zone.isPointInside(point))
		return false;
	Circle circle(point, radius);
	std::pair<size_t, size_t> central_sector = getSector(point);
	for (size_t i = central_sector.first - 1; i <= central_sector.first + 1; ++i) {
		for (size_t j = central_sector.second - 1; j <= central_sector.second + 1; ++j) {
			for (const Point& candidate : base[i][j]) {
				if (circle.isPointInside(candidate))
					return true;
			}
		}
	}
	return false;
}

std::vector<Point> NeighboursBase::getNeighbours(const Point& point, double radius) const {
	std::vector<Point> result;
	if (!zone.isPointInside(point))
		return result;
	Circle circle(point, radius);
	std::pair<size_t, size_t> central_sector = getSector(point);
	for (size_t i = central_sector.first - 1; i <= central_sector.first + 1; ++i) {
		for (size_t j = central_sector.second - 1; j <= central_sector.second + 1; ++j) {
			for (const Point& candidate : base[i][j]) {
				if (circle.isPointInside(candidate))
					result.push_back(candidate);
			}
		}
	}
	return result;
}