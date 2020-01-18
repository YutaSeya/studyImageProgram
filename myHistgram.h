#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>

std::vector<std::vector<float>> myCalcHistogram(cv::Mat data, int8_t color = 1);
cv::Mat myPaintHistgram(std::vector<std::vector<float>> data, int8_t color = 1);
cv::Mat myToneCurve(cv::Mat data, float tilt, int x_offset, int y_offset, int8_t color = 1);
cv::Mat myConvertionLut(cv::Mat data, std::vector<int>lut, int8_t color = 1);