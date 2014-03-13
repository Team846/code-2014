#include "FaceHotGoal.h"

HotGoal::Side FaceHotGoal::m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
int FaceHotGoal::m_hotGoalPos[] = { -1, 1, 0 }; // left, right, none

double FaceHotGoal::s_turnDegrees = 30;

FaceHotGoal::FaceHotGoal() :
	Turn(),
	Configurable("FaceHotGoal")
{
	HotGoal::Side currentSide = SensorFactory::GetHotGoal()->GetActiveSide();
	
	int relativePos = m_hotGoalPos[currentSide] - m_hotGoalPos[m_lastHotGoalSide];
	
	setAngle(relativePos * s_turnDegrees);
	setMaxSpeed(1.0);
	setErrorThreshold(0.5);

	m_lastHotGoalSide = currentSide;
}

void FaceHotGoal::Configure()
{
	s_turnDegrees = GetConfig("turn_degrees", 30.0);
}
