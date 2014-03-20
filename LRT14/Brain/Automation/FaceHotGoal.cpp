#include "FaceHotGoal.h"

#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::IO;

HotGoal::Side FaceHotGoal::m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
int FaceHotGoal::m_hotGoalPos[] = { -1, 1, 0 }; // left, right, none

double FaceHotGoal::s_turnDegrees = 30;

FaceHotGoal::FaceHotGoal() :
	JitterTurn(0.0, 0.0),
	Configurable("FaceHotGoal")
{
	m_turnToCenter = false;
}

FaceHotGoal::FaceHotGoal(bool clear, bool turnToCenter) :
	JitterTurn(0.0, 0.0),
	Configurable("FaceHotGoal")
{
	if(clear)
	{
		m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
	}
	// else wtf r u doin son
	
	m_turnToCenter = turnToCenter;
}

FaceHotGoal::FaceHotGoal(bool clear, bool turnToCenter, double randLow, double randHigh) :
	JitterTurn(randLow, randHigh),
	Configurable("FaceHotGoal")
{
	if(clear)
	{
		m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
	}
	// else wtf r u doin son
	
	m_turnToCenter = turnToCenter;
}

bool FaceHotGoal::Start()
{
	HotGoal::Side currentSide = SensorFactory::GetHotGoal()->GetActiveSide();

	// if we already had a hot goal, just turn to the other side.
	if(m_lastHotGoalSide != HotGoal::NONE_ACTIVE)
	{
		if(m_lastHotGoalSide == HotGoal::LEFT) currentSide = HotGoal::RIGHT;
		if(m_lastHotGoalSide == HotGoal::RIGHT) currentSide = HotGoal::LEFT;
	}
	else
	{
		// if we didn't detect anything go with left
		currentSide = (currentSide == HotGoal::NONE_ACTIVE) ? HotGoal::LEFT : currentSide;
	}

	// are we force turning back to center?
	if(m_turnToCenter) currentSide = HotGoal::NONE_ACTIVE;
	
	int relativePos = m_hotGoalPos[currentSide] - m_hotGoalPos[m_lastHotGoalSide];
	double angle = relativePos * s_turnDegrees * -1;
	
	setAngle(angle);
	setMaxSpeed(1.0);
	setErrorThreshold(0.5);

	m_lastHotGoalSide = currentSide;
	
	JitterTurn::Start();
	
	return true;
}

void FaceHotGoal::Configure()
{
	s_turnDegrees = GetConfig("turn_degrees", 30.0);
}
