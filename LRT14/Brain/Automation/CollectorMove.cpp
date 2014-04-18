#include "CollectorMove.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../Config/DriverStationConfig.h"
#include "../../RobotState.h"
#include "../Events/JoystickReleasedEvent.h"
#include "../../DriverStation/LRTDriverStation.h"

CollectorMove::CollectorMove(bool down) :
	Automation("CollectorMove")
{
	m_collectorArm = CollectorArmData::Get();
	m_down = down;
}

void CollectorMove::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
}

bool CollectorMove::Start()
{
	return true;
}

bool CollectorMove::Run()
{
	CollectorArmData::Position desired = m_down ? CollectorArmData::COLLECT : CollectorArmData::STOWED;
	m_collectorArm->SetDesiredPosition(desired);
	if (m_collectorArm->GetCurrentPosition() == desired)
		return true;
	return false;
}

bool CollectorMove::Abort()
{
	return true;
}

