#pragma once

#include "geometry/basics.h"

#include <vector>
#include <string>
#include <memory>


namespace Plotter {

class Image {
private:
	void* buffer = nullptr;
	int size = 0;

public:
	Image() = default;
	Image(void* buffer, int size);
	Image(const Image&) = delete;
	Image(Image&&);
	~Image();

	Image& operator=(const Image&) = delete;
	Image& operator=(Image&&);

	template<typename ReturnType>
	ReturnType* getBuffer() const {
		return reinterpret_cast<ReturnType*>(buffer);
	}

	int getSize() const;
	bool isValid() const;
	void save(const std::string& filename) const;
};


Image plot(size_t width, size_t height, const std::vector<SegmentedLine>& lines);

}