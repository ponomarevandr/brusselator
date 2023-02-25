#include "plotter.h"

#include <memory>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TImageDump.h"


namespace Plotter {

std::string plot(size_t width, size_t height, const std::vector<SegmentedLine>& lines) {
	auto canvas = std::make_unique<TCanvas>("canvas", "graph", 0, 0, width, height);
	canvas->SetGrid();
	auto multigraph = std::make_unique<TMultiGraph>();
	for (const SegmentedLine& line : lines) {
		TGraph* graph = new TGraph(line.size(), line.xsBuffer(), line.ysBuffer());
		graph->SetLineColor(kRed);
		multigraph->Add(graph);
	}
	multigraph->Draw("AL");
	auto image_dump = std::make_unique<TImageDump>("graph.png");
	canvas->Paint();
	image_dump->Close();
	return "graph.png";
}

}