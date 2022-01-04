#include <opencv2/opencv.hpp>
#include <iostream>
#include <array>
#include <vector>

#include "kdtree.h"

using namespace std;
using namespace cv;

class MyPoint : public std::array<double, 2>
{
public:

	static const int DIM = 2;
	MyPoint() {}
	MyPoint(double x, double y)
	{
		(*this)[0] = x;
		(*this)[1] = y;
	}

	operator cv::Point2d() const { return cv::Point2d((*this)[0], (*this)[1]); }
};

int main(int argc, char** argv)
{
	const int seed = argc > 1 ? std::stoi(argv[1]) : 0;
	srand(seed);

	const int width = 500;		
	const int height = 500;
	Mat img = imread("C:/Users/Admin/source/repos/K-d tree visualization/map.png", 1);

	int npoints;
	cout << "Vui long nhap so diem: ";
	cin >> npoints;
	std::vector<MyPoint> points(npoints);
	for (int i = 0; i < npoints; i++)
	{
		const int x = rand() % width;
		const int y = rand() % height;
		points[i] = MyPoint(x, y);
	}

	for (const auto& pt : points)
		cv::circle(img, cv::Point2d(pt), 1, cv::Scalar(0, 255, 255), -1);


	kdt::KDTree<MyPoint> kdtree(points);

	const MyPoint query(0.5 * width, 0.5 * height);
	cv::circle(img, cv::Point2d(query), 1, cv::Scalar(0, 0, 255), -1);


	const cv::Mat I0 = img.clone();
	const int idx = kdtree.nnSearch(query);
	cv::circle(I0, cv::Point2d(points[idx]), 1, cv::Scalar(255, 255, 0), -1);
	cv::line(I0, cv::Point2d(query), cv::Point2d(points[idx]), cv::Scalar(255, 255, 255));


	const cv::Mat I2 = img.clone();
	double radius;
	cout << "vui long nhap do cho viec tim kiem: ";
	cin >> radius;
	const std::vector<int> radIndices = kdtree.radiusSearch(query, radius);
	for (int i : radIndices)
		cv::circle(I2, cv::Point2d(points[i]), 1, cv::Scalar(255, 255, 0), -1);
	cv::circle(I2, cv::Point2d(query), cvRound(radius), cv::Scalar(0, 0, 255));


	cv::imshow("Nearest neigbor search", I0);
	cv::imshow("Radius search", I2);
	cv::waitKey(0);
	return 0;
}