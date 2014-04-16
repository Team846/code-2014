#include "HumanLoad.h"
#include "../../Config/ConfigPortMappings.h"
#include "../Events/JoystickReleasedEvent.h"
#include "../../DriverStation/LRTDriverStation.h"

HumanLoad::HumanLoad() :
	Automation("HumanLoad"),
	Configurable("HumanLoad")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_loaderData = LauncherLoaderData::Get();
	m_pressurePlate = PressurePlateData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
	m_hasBall = false;
}

void HumanLoad::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::PRESSURE_PLATE);
}

bool HumanLoad::Start()
{
	m_hasBall = false;
	return true;
}

bool HumanLoad::Run()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
	m_collectorRollers->SetRunning(true);
	m_collectorRollers->SetSpeed(m_rollerSpeed);
	if (m_proximity->Get() == 1)
	{
		m_collectorRollers->SetRunning(false);
		m_hasBall = true;
	}
	if (!m_hasBall)
	{
		m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
		m_loaderData->SetUnload(true);
	}
	else
	{
		m_collectorRollers->SetDirection(CollectorRollersData::FORWARD);
		m_loaderData->SetUnload(false);
	}
	m_pressurePlate->SetPressure(false);
	return false;
}

bool HumanLoad::Abort()
{
	if (m_hasBall && dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())
			&& dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())->GetButton() == DriverStationConfig::JoystickButtons::HUMAN_LOAD
			&& dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())->GetJoystick() == LRTDriverStation::Instance()->GetOperatorStick())
		return false;
	
	m_loaderData->SetUnload(false);
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(false);
	m_pressurePlate->SetPressure(true);
	return true;
}

void HumanLoad::Configure()
{
	m_rollerSpeed = GetConfig("roller_speed", 1.0);
}
