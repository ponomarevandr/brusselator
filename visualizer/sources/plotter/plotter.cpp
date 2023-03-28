#include "plotter.h"

#include <stdio.h>
#include <cstdlib>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TImage.h"


Plotter::Image::Image(void* buffer, int size): buffer(buffer), size(size) {}

Plotter::Image::Image(Image&& other): buffer(other.buffer), size(other.size) {
	other.buffer = nullptr;
	other.size = 0;
}

Plotter::Image::~Image() {
	if (buffer)
		free(buffer);
}

Plotter::Image& Plotter::Image::operator=(Image&& other) {
	if (buffer)
		free(buffer);
	buffer = other.buffer;
	size = other.size;
	other.buffer = nullptr;
	other.size = 0;
	return *this;
}

int Plotter::Image::getSize() const {
	return size;
}

bool Plotter::Image::isValid() const {
	return buffer;
}

void Plotter::Image::save(const std::string& filename) const {
	FILE* out_file;
    out_file = fopen(filename.c_str(), "wb");
    fwrite(buffer, 1, size, out_file);
    fclose(out_file);
}


const int Plotter::ROOT_COLOR[7] = {
	kRed, kYellow, kGreen, kCyan, kBlue, kMagenta, kBlack
};

Plotter::Plotter(size_t image_width, size_t image_height): image_width(image_width),
	image_height(image_height) {}

void Plotter::addPortrait(const std::vector<SegmentedLine>& portrait, Color color) {
	portraits.push_back(portrait);
	colors.push_back(color);
}

Plotter::Image Plotter::getImage() {
	auto canvas = std::make_unique<TCanvas>("canvas", "graph", 0, 0, image_width, image_height);
	canvas->SetGrid();
	auto multigraph = std::make_unique<TMultiGraph>();
	for (size_t i = 0; i < portraits.size(); ++i) {
		for (size_t j = 0; j < portraits[i].size(); ++j) {
			std::vector<double> xs = segmentedLineXs(portraits[i][j]);
			std::vector<double> ys = segmentedLineYs(portraits[i][j]);
			TGraph* graph = new TGraph(portraits[i][j].size(), xs.data(), ys.data());
			graph->SetLineColor(ROOT_COLOR[static_cast<size_t>(colors[i])]);
			graph->SetLineWidth(2);
			multigraph->Add(graph);
		}
	}
	multigraph->Draw("AL");
	std::unique_ptr<TImage> image(TImage::Create());
	image->FromPad(canvas.get());
	char* buffer;
	int size;
	image->GetImageBuffer(&buffer, &size);
	return Image(buffer, size);
}