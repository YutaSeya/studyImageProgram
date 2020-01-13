#include "myAffin.h"

#define PI 3.1415926535f

using namespace cv;
using namespace std;

/**
* @brief 回転方向のアフィン変換を行う。
*/
Mat myAffinRotation(Mat data, float deg)
{
	int width = data.size().width;
	int height = data.size().height;

	int rotation_width = 0;
	int rotation_height = 0;
	if (width > height) {
		rotation_width = width;
		rotation_height = width;
	}
	else {
		rotation_width = height;
		rotation_height = height;
	}

	float rad = deg * PI / 180.0f;

	Mat rotation_out;
	rotation_out = Mat::zeros(Size(rotation_height, rotation_width), CV_8UC3);

	for (int y = 0; y < rotation_height; y++) {
		for (int x = 0; x < rotation_width; x++) {
			float x_rota = (x - rotation_width / 2) * cos(rad) -
				(y - rotation_height / 2) * sin(rad) + width / 2;
			float check_x = x_rota + 0.5;
			// 画像の範囲外の場合はfor loopに戻る
			if (check_x > width || check_x < 0) continue;

			float y_rota = (x - rotation_width / 2) * sin(rad) +
				(y - rotation_height / 2) * cos(rad) + height / 2;
			float check_y = y_rota + 0.5;
			// 画像の範囲外の場合はfor loopに戻る
			if (check_y > height || check_y < 0) continue;

			rotation_out.at<Vec3b>(y, x)[0] = data.at<Vec3b>((int)check_y, (int)check_x)[0];
			rotation_out.at<Vec3b>(y, x)[1] = data.at<Vec3b>((int)check_y, (int)check_x)[1];
			rotation_out.at<Vec3b>(y, x)[2] = data.at<Vec3b>((int)check_y, (int)check_x)[2];
		}
	}

	return rotation_out;

}

void myAffinResize(Mat data, float gain)
{

}