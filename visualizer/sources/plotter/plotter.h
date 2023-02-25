#pragma once

#include "geometry/basics.h"

#include <vector>
#include <string>


namespace Plotter {

std::string plot(size_t width, size_t height, const std::vector<SegmentedLine>& lines);

}