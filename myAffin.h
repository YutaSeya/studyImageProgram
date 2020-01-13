#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>

#include <cmath>

cv::Mat myAffinRotation(cv::Mat data, float deg);
void myAffinResize(cv::Mat data, float gain);