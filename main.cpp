#include <vector>
#include <iostream>
#include <delaunayTriangulation/delaunay.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> // drawing shapes

int main() {
	int N = 0; // количество точек в триангуляции
	std::cin >> N;
	std::vector<Point> input;
	for (int i = 0; i < N; i++) {
		double x = 0;
		double y = 0;
		std::cin >> x >> y;
		input.push_back(Point(x, y));
	}
	std::vector<Triangle> result = getTriangulation(input);

	std::cout << "Amount of triangles in triangulation = " << result.size() << std::endl;
	for (int i = 0; i < result.size(); i++) {
		std::cout << i + 1 << ": " << "A" << i << "(" << result[i].A_->x_ << ", " << result[i].A_->y_ << ")"
			<< "     B" << i << "(" << result[i].B_->x_ << ", " << result[i].B_->y_ << ")"
			<< "     C" << i << "(" << result[i].C_->x_ << ", " << result[i].C_->y_ << ")" << std::endl;
	}

	cv::Mat image(2000, 2000, CV_8UC3, cv::Scalar(0, 0, 0));
	if (!image.data) {
		std::cout << "Could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < result.size(); i++) {

		cv::Point p1(result[i].A_->x_, result[i].A_->y_), p2(result[i].B_->x_, result[i].B_->y_), p3(result[i].C_->x_, result[i].C_->y_);
		cv::Scalar colorLine(0, 255, 0);
		int thicknessLine = 2;
		cv::line(image, p1, p2, colorLine, thicknessLine);
		cv::line(image, p2, p3, colorLine, thicknessLine);
		cv::line(image, p3, p1, colorLine, thicknessLine);
	}
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display window", image);

	cv::waitKey(0);
}