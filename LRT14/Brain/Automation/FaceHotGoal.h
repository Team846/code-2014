#ifndef FACE_HOT_GOAL_H_
#define FACE_HOT_GOAL_H_

#include "Automation.h"
#include "Turn.h"
#include "../../Sensors/CheesyVisionServer.h"

class FaceHotGoal : public Turn
{
public:
	FaceHotGoal(std::string startSide, double turnDegrees);
	
	bool Start();
	
private:
	CheesyVisionServer* m_hotGoal;
	double m_turnDegrees;
	std::string m_startSide;
};

#endif
