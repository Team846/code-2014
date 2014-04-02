/*
 * HotGoal.cpp
 *
 *  Created on: Mar 19, 2014
 *      Author: manoj
 */
#include "HotGoal.h"

HotGoal::HotGoal()
{
#ifdef GUI

	valThresh = hueMax = hueMin = satThresh = 0;
	namedWindow("value threshold");
	createTrackbar("thresh val", "value threshold", &valThresh,255);

	namedWindow("sat threshold");
	createTrackbar("thresh val", "sat threshold", &satThresh, 255);
	//setup netclient
#endif

#ifndef GUI
	sockaddr_in crio;


	m_client = new Rhesus::Messenger::NetClient();
	//m_client
#endif

	//setup vidcap
	m_cap = new VideoCapture(0); //will log init errors, its an opencv bug

	if(!m_cap->isOpened())
	{
		printf("[ERROR] Failed to open PS EYE");
		exit(1);
	}

	printf("hue threshold: %d\nsat threshold: %d\nval threshold: %d\n", HUE_THRESH, SAT_THRESH, VAL_THRESH);


}

HotGoal::Status HotGoal::countPixels (Mat &image)
{

	int leftPixels=0, rightPixels=0;

	for(int row = 0; row < image.rows;row++)
	{
		for(int col = 0; col < image.cols;col++)
		{
			if(image.at<unsigned char>(row,col) != 0 && col < image.cols/2)
				leftPixels++;
			else if(image.at<unsigned char>(row,col) != 0 && col >image.cols/2)
				rightPixels++;
		}
	}

	printf("left: %d right: %d\n", leftPixels, rightPixels);


	if(leftPixels == 0 && rightPixels == 0)
		return HotGoal::NO_PIXELS;

	else if(leftPixels > rightPixels)
		return HotGoal::LEFT;
	else
		return HotGoal::RIGHT;
}


void HotGoal::Tick()
{
	Mat hsvFrame;

		Mat hsvChannels[3];
		Mat hsvThresholded[3];
		Mat interFrame;

		//grab frame, perform in-place colorspace conversion, and split channels
		(*m_cap) >> hsvFrame;
		cvtColor(hsvFrame, hsvFrame, CV_RGB2HSV);
		cv::split(hsvFrame, hsvChannels);

#ifdef GUI
		//hue
		//cv::inRange(hsvChannels[0], hueMin, hueMax, hsvThresholded[0]);

		//sat
		cv::threshold(hsvChannels[1],  hsvThresholded[1], satThresh, 255, THRESH_BINARY);

		//val
		cv::threshold(hsvChannels[2], hsvThresholded[2],valThresh, 255, THRESH_BINARY);
		cv::bitwise_and(hsvThresholded[1], hsvThresholded[2], interFrame);

		imshow("value threshold", hsvThresholded[2]);
		//imshow("hue threshold", hsvThresholded[0]);
		imshow("sat threshold", hsvThresholded[1]);
		Size size = interFrame.size();
		line(interFrame, Point(size.width/2,0),Point(size.width/2,size.height),Scalar(255,0,255));
#endif

#ifndef GUI
		//hue
				//cv::inRange(hsvChannels[0], hueMin, hueMax, hsvThresholded[0]);

				//sat
				cv::threshold(hsvChannels[1],  hsvThresholded[1], VAL_THRESH, 255, THRESH_BINARY);

				//val
				cv::threshold(hsvChannels[2], hsvThresholded[2],SAT_THRESH, 255, THRESH_BINARY);
				cv::bitwise_and(hsvThresholded[1], hsvThresholded[2], interFrame);
#endif
				HotGoal::Status result = countPixels(interFrame);

#ifdef GUI

		if(result == HotGoal::NO_PIXELS)
		{
			putText(interFrame, "NO PIXELS" ,Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,0));
		}
		if(result == HotGoal::LEFT)
		{
			putText(interFrame, "LEFT" ,Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,5,10));
		}
		if(result == HotGoal::RIGHT)
		{
			putText(interFrame, "RIGHT" ,Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,10));
		}

		imshow("FINAL FRAME", interFrame);
#endif


}



HotGoal::~HotGoal()
{
	//abort netclient
	m_cap->release();
}




