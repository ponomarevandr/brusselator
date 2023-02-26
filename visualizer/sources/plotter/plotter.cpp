#include "plotter.h"

#include <stdio.h>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TImage.h"


namespace Plotter {

Image::Image(char* buffer, int size): buffer(buffer), size(size) {}

int Image::getSize() const {
	return size;
}

bool Image::isValid() const {
	return buffer.get();
}

void Image::save(const std::string& filename) const {
	FILE* out_file;
    out_file = fopen(filename.c_str(), "wb");
    fwrite(buffer.get(), 1, size, out_file);
    fclose(out_file);
}


Image plot(size_t width, size_t height, const std::vector<SegmentedLine>& lines) {
	auto canvas = std::make_unique<TCanvas>("canvas", "graph", 0, 0, width, height);
	canvas->SetGrid();
	auto multigraph = std::make_unique<TMultiGraph>();
	for (const SegmentedLine& line : lines) {
		TGraph* graph = new TGraph(line.size(), line.xsBuffer(), line.ysBuffer());
		graph->SetLineColor(kRed);
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