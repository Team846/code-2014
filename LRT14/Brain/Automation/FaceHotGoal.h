#ifndef FACE_HOT_GOAL_H_
#define FACE_HOT_GOAL_H_

#include "Automation.h"

#include "JitterTurn.h"

#include "../../Config/Configurable.h"

class CheesyVisionServer;

// TODO: clean this up later
namespace HotGoal
{
	enum Side
	{
		LEFT = 0,
		NONE_ACTIVE = 1,
		RIGHT = 2
	};
}

class FaceHotGoal : public JitterTurn
{
public:
	FaceHotGoal();
	FaceHotGoal(double turnDegrees);
	FaceHotGoal(double turnDegrees, bool clear, bool turnToCenter);
	FaceHotGoal(double turnDegrees, bool clear, bool turnToCenter, double randLow, double randHigh);
	
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

	CheesyVisionServer* m_hotGoal;
	double m_turnDegrees;
	bool m_turnToCenter;
};

#endif
