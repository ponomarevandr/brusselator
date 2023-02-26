#pragma once

#include "geometry/basics.h"

#include <vector>
#include <string>
#include <memory>


namespace Plotter {

class Image {
private:
	std::unique_ptr<char> buffer;
	int size;

public:
	Image(char* buffer, int size);

	template<typename ReturnType>
	ReturnType* getBuffer() const {
		return reinterpret_cast<ReturnType*>(reinterpret_cast<void*>(buffer.get()));
	}

	int getSize() const;
};

Image plot(size_t width, size_t height, const std::vector<SegmentedLine>& lines);

}