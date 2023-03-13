#include "plotter.h"

#include <stdio.h>
#include <cstdlib>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TImage.h"


namespace Plotter {

Image::Image(void* buffer, int size): buffer(buffer), size(size) {}

Image::Image(Image&& other): buffer(other.buffer), size(other.size) {
	other.buffer = nullptr;
	other.size = 0;
}

Image::~Image() {
	if (buffer)
		free(buffer);
}

Image& Image::operator=(Image&& other) {
	if (buffer)
		free(buffer);
	buffer = other.buffer;
	size = other.size;
	other.buffer = nullptr;
	other.size = 0;
	return *this;
}

int Image::getSize() const {
	return size;
}

bool Image::isValid() const {
	return buffer;
}

void Image::save(const std::string& filename) const {
	FILE* out_file;
    out_file = fopen(filename.c_str(), "wb");
    fwrite(buffer, 1, size, out_file);
    fclose(out_file);
}


Image plot(size_t width, size_t height, const std::vector<SegmentedLine>& lines) {
	auto canvas = std::make_unique<TCanvas>("canvas", "graph", 0, 0, width, height);
	canvas->SetGrid();
	auto multigraph = std::make_unique<TMultiGraph>();
	for (size_t i = 0; i < lines.size(); ++i) {
		std::vector<double> xs = segmentedLineXs(lines[i]);
		std::vector<double> ys = segmentedLineYs(lines[i]);
		TGraph* graph = new TGraph(lines[i].size(), xs.data(), ys.data());
		if (i == lines.size() - 1) {
			graph->SetLineColor(kBlue);
		} else {
			graph->SetLineColor(kRed);
		}
		graph->SetLineWidth(2);
		multigraph->Add(graph);
	}
	multigraph->Draw("AL");
	std::unique_ptr<TImage> image(TImage::Create());
	image->FromPad(canvas.get());
	char* buffer;
	int size;
	image->GetImageBuffer(&buffer, &size);
	return Image(buffer, size);
}

}