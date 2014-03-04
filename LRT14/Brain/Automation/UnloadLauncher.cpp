#include "UnloadLauncher.h"
#include "../../Config/ConfigPortMappings.h"

UnloadLauncher::UnloadLauncher() :
	Automation("UnloadLauncher"),
	Configurable("UnloadLauncher")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_loaderData = LauncherLoaderData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
	m_hasBall = false;
}

void UnloadLauncher::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
}

bool UnloadLauncher::Start()
{
	m_hasBall = false;
	return true;
}

bool UnloadLauncher::Run()
{
	m_loaderData->SetPurge(true);
	if (m_loaderData->IsLoadingComplete())
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
		m_collectorRollers->SetRunning(true);
		m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
		m_collectorRollers->SetSpeed(1.0);
		
		if (m_proximity->Get() == 1)
		{
			m_collectorRollers->SetRunning(false);
			m_hasBall = true;
		}
		else
		{
			m_hasBall = false;
		}
	}
	return false;
}

bool UnloadLauncher::Abort()
{
	if (m_hasBall && dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())
			&& dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())->GetButton() == DriverStationConfig::JoystickButtons::UNLOAD_LAUNCHER
			&& dynamic_cast<JoystickReleasedEvent*>(GetAbortEvent())->GetJoystick() == LRTDriverStation::Instance()->GetOperatorStick())
		return false;
	
	m_loaderData->SetPurge(false);
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(false);
	return true;
}

void UnloadLauncher::Configure()
{
}
