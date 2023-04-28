#include <vector>
#include <cmath>
#include <iostream>

struct Point {
	double x_ = 0;
	double y_ = 0;
	Point(double x, double y) {
		x_ = x;
		y_ = y;
	}
};
struct Triangle;

struct Edge {
	Point* A_;
	Point* B_;

	bool good = true;
	bool super = false;

	Edge() = default;
	Edge(Point* A, Point* B) {
		A_ = A;
		B_ = B;
		if ((A->x_ == -1e9 || A->x_ == 1e9 || A->y_ == 1e9) || (B->x_ == -1e9 || B->x_ == 1e9 || B->y_ == 1e9)) {
			super = true;
		}
	}

	bool operator==(const Edge& other) {
		if ((A_ == other.A_ && B_ == other.B_) || (A_ == other.B_ && B_ == other.A_)) {
			return true;
		}
		return false;
	}
};


struct Triangle {
	Point* A_;
	Point* B_;
	Point* C_;
	Edge AB_;
	Edge BC_;
	Edge AC_;

	int count_bad = 0;// ≈сли вдруг треугольник станет "плохим", то это будет количество его "плохих" соседей

	Triangle(Point* A, Point* B, Point* C) {
		A_ = A;
		B_ = B;
		C_ = C;
		AB_ = Edge(A, B);
		BC_ = Edge(B, C);
		AC_ = Edge(A, C);
	}
};

bool InCircle(const Triangle& tr, const Point& p) {
	double A_x = tr.A_->x_;
	double A_y = tr.A_->y_;
	double B_x = tr.B_->x_;
	double B_y = tr.B_->y_;
	double C_x = tr.C_->x_;
	double C_y = tr.C_->y_;

	double d = (A_x * (B_y - C_y) + B_x * (C_y - A_y) + C_x * (A_y - B_y));
	double x = ((A_x * A_x + A_y * A_y) * (B_y - C_y) + (B_x * B_x + B_y * B_y) * (C_y - A_y) + (C_x * C_x + C_y * C_y) * (A_y - B_y)) / (2 * d);
	double y = ((A_x * A_x + A_y * A_y) * (C_x - B_x) + (B_x * B_x + B_y * B_y) * (A_x - C_x) + (C_x * C_x + C_y * C_y) * (B_x - A_x)) / (2 * d);
	double r = sqrt((x - A_x) * (x - A_x) + (y - A_y) * (y - A_y));
	double dist = sqrt((p.x_ - x) * (p.x_ - x) + (p.y_ - y) * (p.y_ - y));
	return dist <= r;
}

bool Check(const Triangle& lhs, const Triangle& rhs) {
	Point* A = lhs.A_;
	Point* B = lhs.B_;
	Point* C = lhs.C_;

	Point* D = rhs.A_;
	Point* E = rhs.B_;
	Point* F = rhs.C_;

	if (((A == D) || (A == E) || (A == F)) && ((B == D) || (B == E) || (B == F))) {
		return true;
	}
	else if (((A == D) || (A == E) || (A == F)) && ((C == D) || (C == E) || (C == F))) {
		return true;
	}
	else if (((B == D) || (B == E) || (B == F)) && ((C == D) || (C == E) || (C == F))) {
		return true;
	}
	else {
		return false;
	}
}



std::vector<Triangle> getTriangulation(std::vector<Point>& points) {
	points.insert(points.begin(), Point(-1e9, -1e9));
	points.insert(points.begin() + 1, Point(1e9, -1e9));
	points.insert(points.begin() + 2, Point(0, 1e9));

	std::vector<Triangle> result;
	result.push_back(Triangle(&points[0], &points[1], &points[2]));

	for (int i = 3; i < points.size(); i++) {
		Point& P = points[i];
		std::vector<int> bad_triangles;
		std::vector<Triangle> new_result;
		for (int j = 0; j < result.size(); j++) {
			if (InCircle(result[j], P)) {
				bad_triangles.push_back(j);
			}
			else {
				new_result.push_back(result[j]);
			}
		}

		for (int j = 0; j < bad_triangles.size() - 1; j++) {
			for (int k = j + 1; k < bad_triangles.size(); k++) {
				Triangle& first = result[bad_triangles[j]];
				Triangle& second = result[bad_triangles[k]];
				if (Check(first, second)) {
					if (first.AB_ == second.AB_) {
						first.AB_.good = false;
						second.AB_.good = false;
					}
					else if (first.AB_ == second.BC_) {
						first.AB_.good = false;
						second.BC_.good = false;
					}
					else if (first.AB_ == second.AC_) {
						first.AB_.good = false;
						second.AC_.good = false;
					}
					else if (first.BC_ == second.AB_) {
						first.BC_.good = false;
						second.AB_.good = false;
					}
					else if (first.BC_ == second.BC_) {
						first.BC_.good = false;
						second.BC_.good = false;
					}
					else if (first.BC_ == second.AC_) {
						first.BC_.good = false;
						second.AC_.good = false;
					}
					if (first.AC_ == second.AB_) {
						first.AC_.good = false;
						second.AB_.good = false;
					}
					else if (first.AC_ == second.BC_) {
						first.AC_.good = false;
						second.BC_.good = false;
					}
					else if (first.AC_ == second.AC_) {
						first.AC_.good = false;
						second.AC_.good = false;
					}
				}
			}
		}
		for (int j = 0; j < bad_triangles.size(); j++) {
			Triangle& bad = result[bad_triangles[j]];
			if (bad.AB_.good == true) {
				new_result.push_back(Triangle(&P, bad.AB_.A_, bad.AB_.B_));
			}
			if (bad.BC_.good == true) {
				new_result.push_back(Triangle(&P, bad.BC_.A_, bad.BC_.B_));
			}
			if (bad.AC_.good == true) {
				new_result.push_back(Triangle(&P, bad.AC_.A_, bad.AC_.B_));
			}
		}
		result = new_result;

	}
	int g = 0;
	while (g < result.size()) {
		if (result[g].A_->x_ == 1e9 || result[g].A_->x_ == -1e9 || result[g].A_->y_ == 1e9 || result[g].A_->y_ == -1e9) {
			result.erase(result.begin() + g);
		}
		else if (result[g].B_->x_ == 1e9 || result[g].B_->x_ == -1e9 || result[g].B_->y_ == 1e9 || result[g].B_->y_ == -1e9) {
			result.erase(result.begin() + g);
		}
		else if (result[g].C_->x_ == 1e9 || result[g].C_->x_ == -1e9 || result[g].C_->y_ == 1e9 || result[g].C_->y_ == -1e9) {
			result.erase(result.begin() + g);
		}
		else {
			g = g + 1;
		}
	}
	return result;
}
