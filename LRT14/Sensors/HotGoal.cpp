#include "HotGoal.h"

using namespace Rhesus::Messenger;

HotGoal::HotGoal()
{
	m_offboard = OffboardCommunication::Instance();
}

HotGoal::~HotGoal()
{
}

HotGoal::Side HotGoal::GetActiveSide()
{
	std::vector<char> buffer;
	m_offboard->Read(OffboardCommunication::HOT_GOAL, buffer);
	return (HotGoal::Side)buffer[0];
}
