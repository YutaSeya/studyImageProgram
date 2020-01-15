#include "myHistgram.h"

using namespace cv;
using namespace std;

void printHistogram(Mat data)
{
	int width = data.size().width;
	int heignt = data.size().height;
	
	vector<Mat> channels(3);

	split(data, channels);

	int red, green, blue;
	int red_hist[256], green_hist[256], blue_hist[256];

	for (int i = 0; i < 256; i++) {
		red_hist[i] = 0;
		green_hist[i] = 0;
		blue_hist[i] = 0;
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < heignt; y++) {
			red = channels[2].at<uchar>(y, x);
			green = channels[1].at<uchar>(y, x);
			blue = channels[0].at<uchar>(y, x);

			red_hist[red]++;
			green_hist[green]++;
			blue_hist[blue]++;
		}
	}

	

}
