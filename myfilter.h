#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>

#define _DEBUG

cv::Mat myAverageFilter(cv::Mat data, cv::Size ksize);
cv::Mat myGaussianFilter(cv::Mat data, cv::Size ksize, float sigma);
cv::Mat mySobelFilter(cv::Mat data, int direction, int color);
cv::Mat myLaplacianFilter(cv::Mat data, int color);