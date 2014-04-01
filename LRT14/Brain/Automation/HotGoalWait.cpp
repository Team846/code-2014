#include "HotGoalWait.h"
#include "../../Sensors/SensorFactory.h"
#include "../../Communication/Dashboard2.h"

HotGoalWait::HotGoalWait()
	: Automation("HotGoalWait")
{
	m_timeout = 8.0; // default (5 seconds + 3 second padding)
}

HotGoalWait::HotGoalWait(double timeout)
	: Automation("HotGoalWait")
{
	m_timeout = timeout;
}

bool HotGoalWait::Start()
{
	m_sw.Start();
	
	return true;
}

bool HotGoalWait::Run()
{
	bool force = m_sw.TotalElapsedSeconds() >= m_timeout;
	
	if(force) Dashboard2::LogW("Autonomous::HotGoalWait(): Time expired!");
	
	return SensorFactory::GetHotGoal()->GetActiveSide() != HotGoal::NONE_ACTIVE || force;
}

bool HotGoalWait::Abort()
{
	return true;
}

void HotGoalWait::AllocateResources()
{
	
}
