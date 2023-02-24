#include "plotter.h"

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TImageDump.h"


namespace Plotter {

std::string plot(size_t width, size_t height, const std::vector<SegmentedLine>& lines,
		bool high_quality) {
	TCanvas* canvas = new TCanvas("canvas", "graph", 0, 0, width, height);
	canvas->SetGrid();
	TMultiGraph* multigraph = new TMultiGraph();
	for (const SegmentedLine& line : lines) {
		TGraph* graph = new TGraph(line.size(), line.xsBuffer(), line.ysBuffer());
		graph->SetLineColor(kRed);
		multigraph->Add(graph);
	}
	multigraph->Draw("AL");
	std::string filename = high_quality ? "graph.png" : "graph.jpeg";
	TImageDump* image_dump = new TImageDump(filename.c_str());
	canvas->Paint();
	image_dump->Close();
	delete canvas;
	delete multigraph;
	delete image_dump;
	return filename;
}

}