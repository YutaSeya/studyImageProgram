#include "myHistgram.h"

using namespace cv;
using namespace std;

vector<vector<float>> myCalcHistogram(Mat data, int8_t color)
{
	int width = data.size().width;
	int height = data.size().height;

	if (color == 1) {
		int red, green, blue;
		int red_hist[256], green_hist[256], blue_hist[256];

		for (int i = 0; i < 256; i++) {
			red_hist[i] = 0;
			green_hist[i] = 0;
			blue_hist[i] = 0;
		}

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				blue = data.at<Vec3b>(y, x)[0];
				green = data.at<Vec3b>(y, x)[1];
				red = data.at<Vec3b>(y, x)[2];

				blue_hist[blue]++;
				green_hist[green]++;
				red_hist[red]++;
			}
		}

		float red_hist_max = 0;
		float green_hist_max = 0;
		float blue_hist_max = 0;

		for (int i = 0; i < 256; i++) {
			if (blue_hist[i] > blue_hist_max) blue_hist_max = blue_hist[i];
			if (green_hist[i] > green_hist_max) green_hist_max = green_hist[i];
			if (red_hist[i] > red_hist_max) red_hist_max = red_hist[i];
			
		}
		vector<vector<float>> hist(3, vector<float>(257));
		for (int i = 0; i < 256; i++) {
			hist[0][i] = (float)(blue_hist[i] / blue_hist_max);
			hist[1][i] = (float)(green_hist[i] / green_hist_max);
			hist[2][i] = (float)(red_hist[i] / red_hist_max);
		}

		// ³‹K‰»‚ÉŽg—p‚µ‚½’l‚ð•Û‘¶‚µ‚Ä‚¨‚­
		hist[0][256] = blue_hist_max;
		hist[1][256] = green_hist_max;
		hist[2][256] = red_hist_max;

		return hist;
	}
	else {
		int gray;
		int gray_hist[256];
		for (int i = 0; i < 256; i++) gray_hist[i] = 0;

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				gray = data.at<uchar>(y, x);
				gray_hist[gray]++;
			}
		}

		float gray_hist_max = 0.0f;

		for (int i = 0; i < 256; i++) {
			if (gray_hist[i] > gray_hist_max) gray_hist_max = gray_hist[i];
		}

		vector<vector<float>> hist(1, vector<float>(257));
		for (int i = 0; i < 256; i++) hist[0][i] = gray_hist[i] / gray_hist_max;

		hist[0][256] = gray_hist_max;
		return hist;
	}	
}

Mat myPaintHistgram(vector<vector<float>> data, int8_t color)
{
	int width = 532;
	int height = 470;
	if (color == 0) {
		height = 180;
	}
	
	Mat histgram = Mat(Size(width, height), CV_8UC3, Scalar(255, 255, 255));

	if (color == 1) {
		for (int i = 0; i < 3; i++) {
			rectangle(histgram, Point(10, 20 + 150 * i), Point(523, 150 + 150 * i), Scalar(220, 220, 220));
		}

		for (int i = 0; i < 256; i++) {
			line(histgram, Point(10 + i * 2, 150), Point(10 + i * 2, 150 - (float)(data[2][i] * 120)), Scalar(0, 0, 255),2);
			line(histgram, Point(10 + i * 2, 300), Point(10 + i * 2, 300 - (float)(data[1][i] * 120)), Scalar(0, 255, 0),2);
			line(histgram, Point(10 + i * 2, 450), Point(10 + i * 2, 450 - (float)(data[0][i] * 120)), Scalar(255, 0, 0),2);
		}
	}
	else {
		rectangle(histgram, Point(10, 20 + 150), Point(523, 150 + 150), Scalar(220, 220, 220));
		for (int i = 0; i < 256; i++) 
			line(histgram, Point(10 + i * 2, 150), Point(10 + i * 2, 150 - (float)(data[0][i] * 120)), Scalar(0, 0, 0),2);
	}

	return histgram;
}

Mat myToneCurve(Mat data, float tilt, int x_offset, int y_offset, int8_t color)
{

	int width = data.size().width;
	int height = data.size().height;

	Mat out;
	out = data.clone();

	vector<int> lut(256);

	for (int i = 0; i < 256; i++) {
		float check = tilt * (i - x_offset) + y_offset;
		if (check >= 255.0f) check = 255.0;
		else if (check < 0.0f) check = 0.0;

		lut[i] = (int)check;
		//cout << lut[i] << endl;
	}

	if (color == 1) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				out.at<Vec3b>(y, x)[0] = lut[data.at<Vec3b>(y, x)[0]];
				out.at<Vec3b>(y, x)[1] = lut[data.at<Vec3b>(y, x)[1]];
				out.at<Vec3b>(y, x)[2] = lut[data.at<Vec3b>(y, x)[2]];
			}
		}
	}
	else {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < 256; y++) {
				out.at<uchar>(y,x) = lut[data.at<uchar>(y, x)];
			}
		}
	}

	return out;
}

Mat myConvertionLut(Mat data, vector<int>lut, int8_t color)
{
	int width = data.size().width;
	int height = data.size().height;

	Mat out;
	out = data.clone();

	if (lut.size() != 256) {
		cout << "not satisfy data size " << lut.size() << endl;
		return data;
	}

	if (color == 1) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				out.at<Vec3b>(y, x)[0] = lut[data.at<Vec3b>(y, x)[0]];
				out.at<Vec3b>(y, x)[1] = lut[data.at<Vec3b>(y, x)[1]];
				out.at<Vec3b>(y, x)[2] = lut[data.at<Vec3b>(y, x)[2]];
			}
		}
	}
	else {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				out.at<uchar>(y, x) = lut[data.at<uchar>(y, x)];
			}
		}
	}

	return out;
}