#include "myAffin.h"

#include <cmath>

#define PI 3.1415926535f

using namespace cv;
using namespace std;

/**
* @brief �摜�̉�]�s���B(�ŋߖT���)
*/
Mat myRotation(Mat data, float deg)
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
	rotation_out = Mat::zeros(Size(rotation_width, rotation_height), CV_8UC3);

	for (int x = 0; x < rotation_width; x++) {
		for (int y = 0; y < rotation_height; y++) {	
			float x_rota = (x - rotation_width / 2) * cos(rad) -
				(y - rotation_height / 2) * sin(rad) + width / 2;
			// �ŋߖT���
			float check_x = roundf(x_rota);
			// �摜�͈̔͊O�̏ꍇ��for loop�ɖ߂�
			if (check_x > width || check_x < 0) continue;

			float y_rota = (x - rotation_width / 2) * sin(rad) +
				(y - rotation_height / 2) * cos(rad) + height / 2;
			// �ŋߖT���
			float check_y = roundf(y_rota);
			// �摜�͈̔͊O�̏ꍇ��for loop�ɖ߂�
			if (check_y > height || check_y < 0) continue;

			rotation_out.at<Vec3b>(y, x)[0] = data.at<Vec3b>((int)check_y, (int)check_x)[0];
			rotation_out.at<Vec3b>(y, x)[1] = data.at<Vec3b>((int)check_y, (int)check_x)[1];
			rotation_out.at<Vec3b>(y, x)[2] = data.at<Vec3b>((int)check_y, (int)check_x)[2];
		}
	}

	return rotation_out;

}

/**
* @brief �T�C�Y�̕ϊ����s���B(���`���[�o�C���j�A])
*/
Mat myResize(Mat data, float x_gain, float y_gain)
{
	int width = data.size().width;
	int height = data.size().height;

	int resize_width = (int)((float)width * x_gain);
	int resize_height = (int)((float)height * y_gain);

	Mat resize_out;
	resize_out = Mat::zeros(Size(resize_width, resize_height), CV_8UC3);

	for (int x = 0; x < resize_width; x++) {
		for (int y = 0; y < resize_height; y++) {
			float x_calc = x / x_gain;
			int x_check = (int)x_calc;
			float dx = x_calc - x_check;
			int x_linear = x_calc + 1;

			if (x_linear == width) x_linear = width - 1;

			float y_calc = y / y_gain;
			int y_check = (int)y_calc;
			float dy = y_calc - y_check;
			int y_linear = y_calc + 1;

			if (y_linear == width) y_linear = width - 1;

			for (int i = 0; i < 3; i++) {
				resize_out.at<Vec3b>(y, x)[i] = data.at<Vec3b>(y_linear, x_linear)[i] * dx * dy;
				resize_out.at<Vec3b>(y, x)[i] += data.at<Vec3b>(y_check, x_linear)[i] * dx * (1-dy);
				resize_out.at<Vec3b>(y, x)[i] += data.at<Vec3b>(y_linear, x_check)[i] * (1-dx) * dy;
				resize_out.at<Vec3b>(y, x)[i] += data.at<Vec3b>(y_check, x_check)[i] * (1-dx) * (1-dy);
			}
			
		}
	}

	return resize_out;
}


/**
* @brief �A�t�B���ϊ����s���B(���`���[�o�C���j�A])
*/
Mat myAffin(Mat data, Mat conv_matrix, float move_x, float move_y)
{
	// �ϊ��s��̍s�񎮂��v�Z���āA�t�s�񂪑��݂��邩�ǂ����̔�����s���B
	double det = determinant(conv_matrix);
	if (det == 0.0f) {
		cout << "�t�s��͑��݂��Ȃ��B���f�[�^��Ԃ�" << endl;
		return data;
	}

	// �t�s������߂�
	Mat inv;

	inv = conv_matrix.inv();

	int width = data.size().width;
	int height = data.size().height;

	Mat affin_out;
	affin_out = Mat::zeros(Size(width, height), CV_8UC3);

	for (int x = 0; x < width; x++) {
		int nowx = x - width / 2;
		for (int y = 0; y < height; y++) {
			int nowy = y - height / 2;

			float rx = (float)nowx * inv.at<float>(0, 0) + (float)nowy * inv.at<float>(0, 1)
				- move_x * inv.at<float>(0, 0) - (float)move_y * inv.at<float>(0, 1) + (float)width / 2;

			// �ŋߖT�⊮
			int dx = (int)roundf(rx);

			// �摜�͈̔͊O�̏ꍇ��for loop�ɂ��ǂ�
			if (dx >= width || dx < 0) continue;

			float ry = (float)nowx * inv.at<float>(1, 0) + (float)nowy * inv.at<float>(1, 1)
				- move_x * inv.at<float>(1, 0) - (float)move_y * inv.at<float>(1, 1) + (float)height / 2;

			// �ŋߖT�⊮
			int dy = (int)roundf(ry);

			// �摜�͈̔͊O�̏ꍇ��for loop�ɂ��ǂ�
			if (dy >= height || dy < 0) continue;

			affin_out.at<Vec3b>(y, x)[0] = data.at<Vec3b>(dy, dx)[0];
			affin_out.at<Vec3b>(y, x)[1] = data.at<Vec3b>(dy, dx)[1];
			affin_out.at<Vec3b>(y, x)[2] = data.at<Vec3b>(dy, dx)[2];
		}
	}
	// �ЂƂ܂��̓f�[�^��Ԃ��Ă���
	return affin_out;
}