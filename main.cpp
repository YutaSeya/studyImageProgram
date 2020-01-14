#include <opencv2/opencv.hpp>

#include <iostream>

#include <cmath>

#include "myfilter.h"
#include "myAffin.h"

using namespace cv;
using namespace std;

//#define __SHOW_ORIGINAL_IMG
//#define __CHECK_OPENCV

//#define __CHECK_AVERAGE
//#define __CHECK_GAUSSIAN
//#define __CHECK_SOBEL
//#define __CHECK_LAPLACIAN
#define __CHECK_GEOMETRY_CONVERTION

int main()
{
	Mat data;
	data = imread("Image/3.jpg");
	if (data.empty()) {
		cout << "Image Data is empty!" << endl;
		return 0;
	}

#if __SHOW_ORIGINAL_IMG 1
	imshow("original img", data);
#endif

#if __CHECK_AVERAGE 1
	Mat ave;
	ave = myAverageFilter(data, Size(3, 3));
	imshow("my average filter", ave);
#endif

#if __CHECK_GAUSSIAN 1
	Mat gauss;
	gauss = myGaussianFilter(data, Size(3, 3), 1.2);
	imshow("my gaussian filter", gauss);
#endif

#if __CHECK_SOBEL 1
	Mat sobel_h, sobel_v;
	sobel_h = mySobelFilter(data, 0, 1);
	sobel_v = mySobelFilter(data, 1, 1);
	imshow("my sobel horizontal", sobel_h);
	imshow("my sobel vertical", sobel_v);
#endif

#if __CHECK_LAPLACIAN 1
	Mat laplacian;
	Mat laplacian_binary;
	laplacian = myLaplacianFilter(data, 1);
	imshow("my laplacian", laplacian);
	Mat cvlap;
	Laplacian(data, cvlap, CV_32F, 3);
	convertScaleAbs(cvlap, cvlap, 1, 0);
	imshow("opencv laplacian", cvlap);
#endif

#if __CHECK_OPENCV 1
	Mat canny;
	Canny(data, canny, 50, 200);
	imshow("opencv canny", canny);
#endif

#if __CHECK_GEOMETRY_CONVERTION 1
	Mat resize_data;
	resize_data = myResize(data, 0.5, 0.5);
	imshow("my resize", resize_data);

	Mat rota = resize_data.clone();
	rota = myRotation(rota, 45);
	imshow("rotaition out", rota);

	Mat conv_matrix;
	conv_matrix = Mat::zeros(Size(2, 2), CV_32F);

	conv_matrix.at<float>(0, 0) = 0.5 * cos(45 * 3.14 / 180);
	conv_matrix.at<float>(0, 1) = 0.5 * -sin(45 * 3.14 / 180);
	conv_matrix.at<float>(1, 0) = 0.5 * sin(45 * 3.14 / 180);
	conv_matrix.at<float>(1, 1) = 0.5 * cos(45 * 3.14 / 180);

	cout << conv_matrix << endl;
	Mat affin_data;
	affin_data = myAffin(data, conv_matrix, 0.0f, 0.0f);
	imshow("affin convertion", affin_data);
#endif

	waitKey();
	return 0;
}