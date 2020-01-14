#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>

#include <cmath>

cv::Mat myRotation(cv::Mat data, float deg);
cv::Mat myResize(cv::Mat data, float x_gain, float y_gain);
cv::Mat myAffin(cv::Mat data, cv::Mat conv_matrix, float move_x, float move_y);