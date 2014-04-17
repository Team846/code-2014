#include "HotGoalWait.h"
#include "../../Sensors/SensorFactory.h"
#include "../../Communication/Dashboard2.h"

HotGoalWait::HotGoalWait(string side,double timeout)
	: Automation("HotGoalWait")
{
	m_side = side;
	m_timeout = timeout;
	m_hotGoal =  CheesyVisionServer::GetInstance();
}

bool HotGoalWait::Start()
{
	m_sw.Start();
	
	return true;
}

bool HotGoalWait::Run()
{
	bool force = m_sw.TotalElapsedSeconds() >= m_timeout; // in case of malfunctioning field or human error
	bool isActive = m_side == "right" ? m_hotGoal->GetRightStatus() : m_hotGoal->GetLeftStatus();
	
	if(force) Dashboard2::LogW("Autonomous::HotGoalWait(): Time expired!");
	
	return isActive|| force;
}

bool HotGoalWait::Abort()
{
	return true;
}

void HotGoalWait::AllocateResources()
{
	
}
