#include "FaceHotGoal.h"

#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::IO;

HotGoal::Side FaceHotGoal::m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
int FaceHotGoal::m_hotGoalPos[] = { -1, 1, 0 }; // left, right, none

double FaceHotGoal::s_turnDegrees = 30;

FaceHotGoal::FaceHotGoal() :
	Turn(),
	Configurable("FaceHotGoal")
{
}

FaceHotGoal::FaceHotGoal(bool clear) :
	Turn(),
	Configurable("FaceHotGoal")
{
	if(clear)
	{
		m_lastHotGoalSide = HotGoal::NONE_ACTIVE;
	}
}

bool FaceHotGoal::Start()
{
	HotGoal::Side currentSide = SensorFactory::GetHotGoal()->GetActiveSide();

	BufferedConsole::Printfln("FaceHotGoal: %d\n", (int)currentSide);
	
	int relativePos = m_hotGoalPos[currentSide] - m_hotGoalPos[m_lastHotGoalSide];
	double angle = relativePos * s_turnDegrees * -1;
	
	BufferedConsole::Printfln("FaceHotGoal: %lf\n", angle);
	
	setAngle(angle);
	setMaxSpeed(1.0);
	setErrorThreshold(0.5);

	m_lastHotGoalSide = currentSide;
	
	Turn::Start();
	
	return true;
}

void FaceHotGoal::Configure()
{
	s_turnDegrees = GetConfig("turn_degrees", 30.0);
}
