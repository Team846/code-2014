#ifndef FACE_HOT_GOAL_H_
#define FACE_HOT_GOAL_H_

#include "Automation.h"

#include "JitterTurn.h"

#include "../../Config/Configurable.h"

#include "../../Sensors/HotGoal.h"
#include "../../Sensors/SensorFactory.h"

class FaceHotGoal : public JitterTurn, public Configurable
{
public:
	FaceHotGoal();
	FaceHotGoal(bool clear, bool turnToCenter);
	FaceHotGoal(bool clear, bool turnToCenter, double randLow, double randHigh);
	
	void Configure();
	
	bool Start();
	
	static void Reset()
	{
		m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
	}
	
	static HotGoal::Side LastHotGoalSide()
	{
		return m_lastHotGoalSide;
	}
	
private:
	static HotGoal::Side m_lastHotGoalSide;
	static int m_hotGoalPos[3];
	
	static double s_turnDegrees;

	bool m_turnToCenter;
};

#endif
