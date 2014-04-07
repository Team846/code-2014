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
	m_pressurePlate = PressurePlateData::Get();
	m_down = down;
}

void CollectorMove::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::PRESSURE_PLATE);
}

bool CollectorMove::Start()
{
	CollectorArmData::Position desired = m_down ? CollectorArmData::COLLECT : CollectorArmData::STOWED;
	m_collectorArm->SetDesiredPosition(desired);
	if (m_collectorArm->GetCurrentPosition() == desired)
		return true;
	return false;
}

bool CollectorMove::Run()
{
	return true;
}

bool CollectorMove::Abort()
{
	return true;
}

