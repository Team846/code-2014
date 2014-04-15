#include "LoadLauncher.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../Config/DriverStationConfig.h"
#include "../../RobotState.h"

LoadLauncher::LoadLauncher() :
	Automation("LoadLauncher"),
	Configurable("LoadLauncher")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_loaderData = LauncherLoaderData::Get();
	m_pressurePlate = PressurePlateData::Get();
	m_launcherProximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_LAUNCHER_PROXIMITY"));
	m_bumperProximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
	m_loadSpeed = 1.0;
	m_pastIntermediate = false;
	m_ballIn = false;
	m_timerStarted = false;
}

void LoadLauncher::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::PRESSURE_PLATE);
}

bool LoadLauncher::Start()
{
	m_pastIntermediate = false;
	m_ballIn = false;
	m_timerStarted = false;
	return true;
}

bool LoadLauncher::Run()
{
	if (!m_loaderData->IsLoadingComplete() && !m_pastIntermediate)
	{
		return false;
	}
	m_pastIntermediate = true;
	m_loaderData->SetLoad(true);
	m_collectorRollers->SetDirection(CollectorRollersData::FORWARD);
	m_collectorRollers->SetSpeed(m_loadSpeed);
	m_pressurePlate->SetPressure(false);
	
	if (m_bumperProximity->Get() == 0)
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
		m_collectorRollers->SetRunning(false);
	}
	else
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
		m_collectorRollers->SetRunning(true);
	}
	if (m_loaderData->IsLoadingComplete())
	{
		if (!m_timerStarted)
		{
			m_timer.Reset();
			m_timer.Start();
			m_timerStarted = true;
		}
		if (m_launcherProximity->Get() == 1 || m_timer.Get() >= m_ballSettleTime)
		{
			m_loaderData->SetLoad(false);
			m_collectorRollers->SetRunning(false);
			m_pressurePlate->SetPressure(true);
			m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
			return true;
		}
	}
	return false;
}

bool LoadLauncher::Abort()
{
	if (m_launcherProximity->Get() == 0 && m_bumperProximity->Get() == 1)
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
	else
		m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(false);
	m_pressurePlate->SetPressure(true);
	m_loaderData->SetLoad(false);
	return true;
}

void LoadLauncher::Configure()
{
	m_loadSpeed = GetConfig("load_speed", 1.0);
	m_ballSettleTime = GetConfig("ball_settle_time", 1.0);
}
