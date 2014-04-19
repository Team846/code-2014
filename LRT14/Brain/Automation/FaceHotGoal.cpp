#include "FaceHotGoal.h"
#include "../../Sensors/CheesyVisionServer.h"

#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::IO;

HotGoal::Side FaceHotGoal::m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
int FaceHotGoal::m_hotGoalPos[] = { -1, 1, 0 }; // left, right, none

// TODO: ctor chaining

FaceHotGoal::FaceHotGoal(double turnDegrees) :
	JitterTurn(0.0, 0.0),
	m_hotGoal(CheesyVisionServer::GetInstance()),
	m_turnDegrees(turnDegrees),
	m_turnToCenter(false)
{
}

FaceHotGoal::FaceHotGoal(double turnDegrees, bool clear, bool turnToCenter) :
	JitterTurn(0.0, 0.0),
	m_hotGoal(CheesyVisionServer::GetInstance()),
	m_turnDegrees(turnDegrees),
	m_turnToCenter(turnToCenter)
{
	if(clear)
	{
		m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
	}
}

FaceHotGoal::FaceHotGoal(double turnDegrees, bool clear, bool turnToCenter, double randLow, double randHigh) :
	JitterTurn(randLow, randHigh),
	m_hotGoal(CheesyVisionServer::GetInstance()),
	m_turnDegrees(turnDegrees),
	m_turnToCenter(turnToCenter)
{
	if(clear)
	{
		m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
	}
}

bool FaceHotGoal::Start()
{
	HotGoal::Side currentSide = HotGoal::NONE_ACTIVE;
	
	if(m_hotGoal->GetLeftStatus() && m_hotGoal->GetRightStatus()) { } // NONE_ACTIVE
	else if(m_hotGoal->GetLeftStatus())
	{
		currentSide = HotGoal::LEFT;
	}
	else if(m_hotGoal->GetRightStatus())
	{
		currentSide = HotGoal::RIGHT;
	}
	else
	{ } // NONE_ACTIVE

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
	double angle = relativePos * m_turnDegrees * -1;
	
	setAngle(angle);
	setMaxSpeed(1.0);
	setErrorThreshold(0.5);

	m_lastHotGoalSide = currentSide;
	
	JitterTurn::Start();
	
	return true;
}
