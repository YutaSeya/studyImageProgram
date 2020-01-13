#include "myfilter.h"

using namespace cv;
using namespace std;

#define PI 3.1415926535f

/**
* @brief 自作平均フィルタ
* @detail 端の計算処理はしていないため、端の画素は入力画像と同じように表示される。
*		  理想は、端のほうも処理をいれることだが、バグフィックスが面倒なためいれていない。
**/
Mat myAverageFilter(Mat data, Size ksize)
{
	int height = data.size().height;
	int width = data.size().width;

	Mat out;

	out = data.clone();

	uint64_t sumR = 0;
	uint64_t sumG = 0;
	uint64_t sumB = 0;

	for (int x = ksize.width / 2; x < width - ksize.width / 2; x++) {
		for (int y = ksize.height / 2; y < height - ksize.height / 2; y++) {
			for (int k = -ksize.width / 2; k <= ksize.width / 2; k++) {
				for (int l = -ksize.height / 2; l <= ksize.height / 2; l++) {
					sumB += data.at<Vec3b>(y + l, x + k)[0];
					sumG += data.at<Vec3b>(y + l, x + k)[1];
					sumR += data.at<Vec3b>(y + l, x + k)[2];
				}
			}

			out.at<Vec3b>(y - ksize.height / 2, x - ksize.width / 2)[0] = sumB / ksize.area();
			out.at<Vec3b>(y - ksize.height / 2, x - ksize.width / 2)[1] = sumG / ksize.area();
			out.at<Vec3b>(y - ksize.height / 2, x - ksize.width / 2)[2] = sumR / ksize.area();
			sumB = 0;
			sumG = 0;
			sumR = 0;
		}
	}

	return out;
}

/**
* @brief 自作ガウシアンフィルタ
* @detail 端の計算処理はしていないため、端の画素は入力画像と同じように表示される。
*		  理想は、端のほうも処理をいれることだが、バグフィックスが面倒なためいれていない。
**/
Mat myGaussianFilter(Mat data, Size ksize, float sigma)
{
	Mat karnel;
	karnel = Mat::zeros(ksize, CV_32F);
	float gause_sum = 0.0f;
	// ガウシアンフィルタ用のカーネルを作成
	for (int x = -ksize.width / 2; x <= ksize.width / 2; x++) {
		for (int y = -ksize.height / 2; y <= ksize.height / 2; y++) {
			karnel.at<float>(y + ksize.height / 2, x + ksize.width / 2) = (float)exp(-((x * x + y * y) / (2 * sigma * sigma))) / (2 * PI * sigma * sigma);
			gause_sum += karnel.at<float>(y + ksize.height / 2, x + ksize.width / 2);
		}
	}

	// スカラ倍をして、和が1になるように落とし込む
	karnel = karnel / gause_sum;

#if _DEBUG 1
	cout << karnel << endl;
#endif

	int height = data.size().height;
	int width = data.size().width;

	Mat out;

	out = data.clone();

	float sumR = 0;
	float sumG = 0;
	float sumB = 0;

	for (int x = ksize.width / 2; x < width - ksize.width / 2; x++) {
		for (int y = ksize.height / 2; y < height - ksize.height / 2; y++) {
			for (int k = -ksize.width / 2; k <= ksize.width / 2; k++) {
				for (int l = -ksize.height / 2; l <= ksize.height / 2; l++) {
					sumB += (float)data.at<Vec3b>(y + l, x + k)[0] * karnel.at<float>(l + ksize.height / 2, k + ksize.width / 2);
					sumG += (float)data.at<Vec3b>(y + l, x + k)[1] * karnel.at<float>(l + ksize.height / 2, k + ksize.width / 2);
					sumR += (float)data.at<Vec3b>(y + l, x + k)[2] * karnel.at<float>(l + ksize.height / 2, k + ksize.width / 2);
				}
			}

			out.at<Vec3b>(y - ksize.height / 2, x - ksize.width / 2)[0] = sumB;
			out.at<Vec3b>(y - ksize.height / 2, x - ksize.width / 2)[1] = sumG;
			out.at<Vec3b>(y - ksize.height / 2, x - ksize.width / 2)[2] = sumR;
			sumB = 0.0f;
			sumG = 0.0f;
			sumR = 0.0f;
		}
	}

	return out;
}

Mat mySobelFilter(Mat data, int direction, int color)
{
	Mat karnel;
	karnel = Mat::zeros(Size(3, 3), CV_32F);
	if (direction == 0) {
		karnel.at<float>(0, 0) = 1;
		karnel.at<float>(0, 2) = -1;
		karnel.at<float>(1, 0) = 2;
		karnel.at<float>(1, 2) = -2;
		karnel.at<float>(2, 0) = 1;
		karnel.at<float>(2, 2) = -1;
	}
	else {
		karnel.at<float>(0, 0) = 1;
		karnel.at<float>(0, 1) = 2;
		karnel.at<float>(0, 2) = 1;
		karnel.at<float>(2, 0) = -1;
		karnel.at<float>(2, 1) = -2;
		karnel.at<float>(2, 2) = -1;
	}


#if _DEBUG 1
	cout << karnel << endl;
#endif

	int height = data.size().height;
	int width = data.size().width;

	Mat out;

	if (color == 0) {
		out = data.clone();

		float sumR = 0;
		float sumG = 0;
		float sumB = 0;

		for (int x = 1; x < width - 1; x++) {
			for (int y = 1; y < height - 1; y++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						sumB += (float)data.at<Vec3b>(y + l, x + k)[0] * karnel.at<float>(l + 1, k + 1);
						sumG += (float)data.at<Vec3b>(y + l, x + k)[1] * karnel.at<float>(l + 1, k + 1);
						sumR += (float)data.at<Vec3b>(y + l, x + k)[2] * karnel.at<float>(l + 1, k + 1);
					}
				}
				if (sumB < 0.0f) sumB = 0;
				if (sumG < 0.0f) sumG = 0;
				if (sumR < 0.0f) sumR = 0;

				out.at<Vec3b>(y - 1, x - 1)[0] = sumB;
				out.at<Vec3b>(y - 1, x - 1)[1] = sumG;
				out.at<Vec3b>(y - 1, x - 1)[2] = sumR;
				sumB = 0.0f;
				sumG = 0.0f;
				sumR = 0.0f;
			}
		}
	}
	else {
		Mat gray_data;

		cvtColor(data, gray_data, CV_BGR2GRAY);
		out = gray_data.clone();
		float sum_data = 0.0f;

		for (int x = 1; x < width - 1; x++) {
			for (int y = 1; y < height - 1; y++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						sum_data += (float)gray_data.at<uchar>(y + l, x + k) * karnel.at<float>(l + 1, k + 1);
					}
				}

				if (sum_data < 0.0f) sum_data = 0.0f;
				out.at<uchar>(y - 1, x - 1) = (uchar)sum_data;
				sum_data = 0.0f;
			}
		}
	}

	return out;
}

Mat myLaplacianFilter(Mat data, int color)
{
	Mat karnel;
	karnel = Mat::zeros(Size(3, 3), CV_32F);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			karnel.at<float>(i, j) = 1;
		}
	}

	karnel.at<float>(1,1) = -8;

#if _DEBUG 1
	cout << karnel << endl;
#endif

	int height = data.size().height;
	int width = data.size().width;

	Mat out;

	if (color == 0) {
		out = data.clone();

		float sumR = 0;
		float sumG = 0;
		float sumB = 0;

		for (int x = 1; x < width - 1; x++) {
			for (int y = 1; y < height - 1; y++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						sumB += (float)data.at<Vec3b>(y + l, x + k)[0] * karnel.at<float>(l + 1, k + 1);
						sumG += (float)data.at<Vec3b>(y + l, x + k)[1] * karnel.at<float>(l + 1, k + 1);
						sumR += (float)data.at<Vec3b>(y + l, x + k)[2] * karnel.at<float>(l + 1, k + 1);
					}
				}
				if (sumB < 0.0f) sumB = 0;
				if (sumG < 0.0f) sumG = 0;
				if (sumR < 0.0f) sumR = 0;

				out.at<Vec3b>(y - 1, x - 1)[0] = sumB;
				out.at<Vec3b>(y - 1, x - 1)[1] = sumG;
				out.at<Vec3b>(y - 1, x - 1)[2] = sumR;
				sumB = 0.0f;
				sumG = 0.0f;
				sumR = 0.0f;
			}
		}
	}
	else {
		Mat gray_data;

		cvtColor(data, gray_data, CV_BGR2GRAY);
		out = gray_data.clone();
		float sum_data = 0.0f;

		for (int x = 1; x < width - 1; x++) {
			for (int y = 1; y < height - 1; y++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						sum_data += (float)gray_data.at<uchar>(y + l, x + k) * karnel.at<float>(l + 1, k + 1);
					}
				}

				if (sum_data < 0.0f) sum_data = 0.0f;
				out.at<uchar>(y - 1, x - 1) = (uchar)sum_data;
				sum_data = 0.0f;
			}
		}
	}

	return out;
}

