#ifndef HOTGOAL_H_
#define HOTGOAL_H_

//#define GUI

#include "Constants.h"

#ifndef GUI
#include "../Networking/NetClient.h"
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cstdio>



using namespace cv;
using namespace Rhesus::Messenger;

/*!
 * @brief Processes vision target and sends result to cRio
 */
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
	NetClient* m_client;
#endif

};




#endif /* HOTGOAL_H_ */
