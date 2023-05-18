#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <delaunayTriangulation/delaunay.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> 

int main(int argc, char* argv[]) {
	std::ifstream ifstrm;
	std::string file = argv[1];
	ifstrm.open(file);
	if (ifstrm.is_open()) {
		std::cout << "File is opened" << std::endl;
	}
	int N = 0;
	ifstrm >> N;
	std::vector<Point> input;
	for (int i = 0; i < N; i++) {
		double x = 0;
		double y = 0;
		ifstrm >> x >> y;
		input.push_back(Point(x, y));
	}
	std::vector<Triangle> result = getTriangulation(input);
}
