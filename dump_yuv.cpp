#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

unsigned char yuvData[1920 * 1080 * 3];
unsigned char *pYuvBuf = yuvData;

static void rgb2nv12(Mat &src, char*filename)
{
	std::vector<cv::Mat> rgbChannels(3);
	split(src, rgbChannels);

	printf("%ld %d %d\n", rgbChannels[0].step[0], rgbChannels[0].cols, rgbChannels[0].rows);
	unsigned char *pR = rgbChannels[2].data;
    unsigned char *pG = rgbChannels[1].data;
    unsigned char *pB = rgbChannels[0].data;
	unsigned char *pY = pYuvBuf;
	unsigned char *pUV;

	for (int i = 0; i < src.rows; i++)
	{
		pUV = pYuvBuf + src.rows*src.cols + (i/2)*src.cols;
		for (int j = 0; j < src.cols; j++)
		{
			unsigned char r = *pR, g = *pG, b = *pB;
			unsigned char Y = (unsigned char)(0.299*r + 0.587*g + 0.144*b);
			*pY++ = Y;
			if (0 == (j%2)) 
			{ 
				unsigned char U = (unsigned char)(0.713*(r - Y) + 128);
				unsigned char V = (unsigned char)(0.564*(b - Y) + 128);
				*pUV++ = V; 
				*pUV++ = U; 
			}
			pR++;
			pG++;
			pB++;
		}
	}
	
	FILE* pFileOut0 = fopen(filename, "wb");
	if (!pFileOut0)
	{
		printf("pFileOut open error \n");
		exit(-1);
	}

	fwrite(pYuvBuf, src.rows*src.cols*src.channels(), 1, pFileOut0);
	fclose(pFileOut0);
}

static void dumpYV12(Mat &src, char*filename)
{
	Mat yuvM;
	cvtColor(src, yuvM, CV_BGR2YUV_I420);

    printf("[%d, %d, %d]\n", yuvM.channels(), yuvM.cols, yuvM.rows);
	memcpy(pYuvBuf, yuvM.data, yuvM.rows*yuvM.cols*yuvM.channels());
	FILE* pFileOut0 = fopen(filename, "wb");
	if (!pFileOut0)
	{
		printf("pFileOut open error \n");
		exit(-1);
	}

	fwrite(pYuvBuf, yuvM.rows*yuvM.cols*yuvM.channels(), 1, pFileOut0);
	fclose(pFileOut0);
}

int main(int argc, char *argv[])
{
	printf("filename: %s\n", argv[1]);
	printf("dump to %s\n", argv[2]);

	Mat src = imread(argv[1]);
	if (src.empty())
	{
		printf("can not open %s\n", argv[1]);
		exit(-1);
	}

	rgb2nv12(src, argv[2]);
	dumpYV12(src, argv[2]);
    return 0;
}


