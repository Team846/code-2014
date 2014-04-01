#include "HotGoalWait.h"
#include "../../Sensors/SensorFactory.h"

HotGoalWait::HotGoalWait()
	: Automation("HotGoalWait")
{
	
}

bool HotGoalWait::Start()
{
	
}

bool HotGoalWait::Run()
{
	return SensorFactory::GetHotGoal()->GetActiveSide() != HotGoal::NONE_ACTIVE;
}

bool HotGoalWait::Abort()
{
	
}

void HotGoalWait::AllocateResources()
{
	
}
