#include "HumanLoad.h"
#include "../../Config/ConfigPortMappings.h"
#include "../Events/JoystickReleasedEvent.h"
#include "../../DriverStation/LRTDriverStation.h"

HumanLoad::HumanLoad() :
	Automation("HumanLoad")
{
	m_collectorArm = CollectorArmData::Get();
	m_ballHolder = BallHolderData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_LAUNCHER_PROXIMITY"));
}

void HumanLoad::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::BALL_HOLDER);
}

bool HumanLoad::Start()
{
	return true;
}

bool HumanLoad::Run()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::INTERMEDIATE);
	m_ballHolder->SetHold(false);
	if (m_proximity->Get() == 1)
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
		m_ballHolder->SetHold(true);
		return true;
	}
	return false;
}

bool HumanLoad::Abort()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_ballHolder->SetHold(true);
	return true;
}
