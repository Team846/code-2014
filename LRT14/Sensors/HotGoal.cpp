#include "HotGoal.h"

HotGoal::HotGoal()
{
	m_offboard = OffboardCommunication::Instance();
}

HotGoal::~HotGoal()
{
	
}

HotGoal::Side HotGoal::GetActiveSide()
{
	return (HotGoal::Side)m_offboard->GetStreamBuffer(OffboardCommunication::HOT_GOAL).at(0);
}
