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
