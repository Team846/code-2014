#include "PassBack.h"
#include "../../Config/ConfigPortMappings.h"
#include "../Events/JoystickReleasedEvent.h"
#include "../../DriverStation/LRTDriverStation.h"

PassBack::PassBack() :
	Automation("PassBack")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_loaderData = LauncherLoaderData::Get();
	m_ballHolder = BallHolderData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_LAUNCHER_PROXIMITY"));
	m_reverse = false;
}

void PassBack::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::BALL_HOLDER);
}

bool PassBack::Start()
{
	m_reverse = false;
	return true;
}

bool PassBack::Run()
{
	if (m_proximity->Get() == 1)
	{
		m_reverse = true;
	}
	m_loaderData->SetPurge(true);
	m_ballHolder->SetHold(false);
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(true);
	m_collectorRollers->SetSpeed(1.0);
	m_collectorRollers->SetDirection(m_reverse ? CollectorRollersData::REVERSE : CollectorRollersData::FORWARD);
	return false;
}

bool PassBack::Abort()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_ballHolder->SetHold(true);
	m_collectorRollers->SetRunning(false);
	m_loaderData->SetPurge(false);
	return true;
}
