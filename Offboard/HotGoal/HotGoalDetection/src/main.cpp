///*
// * main.cpp
// *
// *  Created on: Mar 10, 2014
// *      Author: manoj
// */
//
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <cstdio>
//
//using namespace cv;
//using namespace std;
//
//#define GUI 1
//
//void on_trackbar( int, void* )
//{
//}
//
//int countPixelsMiddle (Mat &image)
//{
//
//	int leftPixels=0, rightPixels=0;
//
//	for(int row = 0; row < image.rows;row++)
//	{
//		for(int col = 0; col < image.cols;col++)
//		{
//			if(image.at<unsigned char>(row,col) != 0 && col < image.cols/2)
//				leftPixels++;
//			else if(image.at<unsigned char>(row,col) != 0 && col >image.cols/2)
//				rightPixels++;
//		}
//	}
//
//	printf("left: %d right: %d\n", leftPixels, rightPixels);
//
//
//	if(leftPixels == 0 && rightPixels == 0)
//		return -1;
//
//	else if(leftPixels > rightPixels)
//		return 0;
//	else
//		return 1;
//}




//int main( int argc, char** argv)
//{
//
//	VideoCapture cap(0);
//
//	if(!cap.isOpened())
//		return -1;
//	int thresh = 0;
//	int hueMin = 0;
//	int hueMax = 0;
//	int satThres = 0;
//
//#ifdef GUI
//	namedWindow("value threshold");
//	createTrackbar("thresh val", "value threshold", &thresh,255);
//
//	namedWindow("sat threshold");
//	createTrackbar("thresh val", "sat threshold", &satThres, 255);
//#endif
//
//	while(1)
//	{
//		Mat hsvFrame;
//
//		Mat hsvChannels[3];
//		Mat hsvThresholded[3];
//		Mat interFrame;
//
//		cap >> hsvFrame;
//		cvtColor(hsvFrame, hsvFrame, CV_RGB2HSV);
//		cv::split(hsvFrame, hsvChannels);
//
//		//hue
//		//cv::inRange(hsvChannels[0], hueMin, hueMax, hsvThresholded[0]);
//
//		//sat
//		cv::threshold(hsvChannels[1],  hsvThresholded[1], satThres, 255, THRESH_BINARY);
//
//		//val
//		cv::threshold(hsvChannels[2], hsvThresholded[2],thresh, 255, THRESH_BINARY);
//		cv::bitwise_and(hsvThresholded[1], hsvThresholded[2], interFrame);
//
//		imshow("value threshold", hsvThresholded[2]);
//		//imshow("hue threshold", hsvThresholded[0]);
//		imshow("sat threshold", hsvThresholded[1]);
//		Size size = interFrame.size();
//		//line(interFrame, Point(size.width/2,0),Point(size.width/2,size.height),Scalar(255,0,255));
//
//		int result = countPixelsMiddle(interFrame);
//
//
//		if(result == -1)
//		{
//			putText(interFrame, "NO PIXELS" ,Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,0));
//		}
//		if(result == 0)
//		{
//			putText(interFrame, "LEFT" ,Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,5,10));
//		}
//		if(result == 1)
//		{
//			putText(interFrame, "RIGHT" ,Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,10));
//		}
//
//		imshow("FINAL FRAME", interFrame);
//
//
//
//
//		//erode + dilate
//		/*cv::erode(interFrame, postErosion, Mat());
//		cv::dilate(postErosion,postDilation,Mat());*/
//
//
//		if(waitKey(30) >= 0) break;
//	}
//}
#include "HotGoal/HotGoal.h"

int main (int argc, char** argv)
{
	HotGoal* hotgoal = new HotGoal();

	while(1)
	{
		hotgoal->Tick();
		if(waitKey(30) >= 0) break;
		usleep(5000);
	}
}




