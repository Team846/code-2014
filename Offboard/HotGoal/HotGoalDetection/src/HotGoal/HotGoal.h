

#ifndef HOTGOAL_H_
#define HOTGOAL_H_

#include "Constants.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../Networking/NetClient.h"

#include <cstdio>

using namespace cv;

/*!
 * @brief Processes vision target and sends result to cRio
 */
#define GUI
class HotGoal
{
public:
	enum Status
	{
		LEFT = 0,
		RIGHT = 1,
		NO_PIXELS = -1
	};
	HotGoal();
	~HotGoal();

	void Tick();

	HotGoal::Status countPixels(Mat &image);
private:

#ifdef GUI
	int valThresh;
	int hueMin;
	int hueMax;
	int satThresh;


#endif
	VideoCapture* m_cap;

#ifndef GUI
	Rhesus::Messenger::NetClient* m_client;
#endif

};




#endif /* HOTGOAL_H_ */
