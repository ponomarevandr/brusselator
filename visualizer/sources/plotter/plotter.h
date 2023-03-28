#pragma once

#include "geometry/basics.h"

#include <vector>
#include <string>
#include <memory>


class Plotter {
public:
	enum class Color {
		RED = 0,
		YELLOW = 1,
		GREEN = 2,
		CYAN = 3,
		BLUE = 4,
		MAGENTA = 5,
		BLACK = 6
	};
	
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

private:
	static const int ROOT_COLOR[7];
	size_t image_width;
	size_t image_height;
	std::vector<std::vector<SegmentedLine>> portraits;
	std::vector<Color> colors;

public:
	Plotter(size_t image_width, size_t image_height);
	void addPortrait(const std::vector<SegmentedLine>& tracks, Color color);
	Image getImage();
};