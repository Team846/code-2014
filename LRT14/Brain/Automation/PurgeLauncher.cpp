#include "PurgeLauncher.h"
#include "../../Config/ConfigPortMappings.h"
#include "../Events/JoystickReleasedEvent.h"
#include "../../DriverStation/LRTDriverStation.h"

PurgeLauncher::PurgeLauncher() :
	Automation("PurgeLauncher"),
	Configurable("PurgeLauncher")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_loaderData = LauncherLoaderData::Get();
	m_ballHolder = BallHolderData::Get();
	m_ballPusher = BallPusherData::Get();
	m_timerStarted = false;
}

void PurgeLauncher::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::BALL_HOLDER);
	AllocateResource(ControlResource::BALL_PUSHER);
}

bool PurgeLauncher::Start()
{
	m_timer.Reset();
	m_timerStarted = false;
	return true;
}

bool PurgeLauncher::Run()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(true);
	m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
	m_collectorRollers->SetSpeed(m_rollerSpeed);
	m_loaderData->SetPurge(true);
	m_ballHolder->SetHold(false);
	m_ballPusher->SetPush(false);
	if (m_loaderData->IsLoadingComplete())
	{
		m_ballPusher->SetPush(true);
		if (!m_timerStarted)
			m_timer.Start();
		m_timerStarted = true;
		if (m_timer.Get() >= m_extendTime)
		{
			m_loaderData->SetPurge(false);
			m_collectorRollers->SetRunning(false);
			m_ballPusher->SetPush(false);
			m_timer.Stop();
			return true;
		}
	}
	return false;
}

bool PurgeLauncher::Abort()
{
	m_loaderData->SetPurge(false);
	m_collectorRollers->SetRunning(false);
	m_ballPusher->SetPush(false);
	return true;
}

void PurgeLauncher::Configure()
{
	m_rollerSpeed = GetConfig("roller_speed", 1.0);
	m_extendTime = GetConfig("extend_time", 0.5);
}
