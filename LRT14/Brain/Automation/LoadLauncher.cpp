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
	m_loadSpeed = 1.0;
	m_pastIntermediate = false;
	m_ticks = 0;
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
	m_ticks = 0;
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
	if (m_ticks >= m_upTicks)
		m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	else
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
	m_collectorRollers->SetRunning(true);
	m_collectorRollers->SetDirection(CollectorRollersData::FORWARD);
	m_collectorRollers->SetSpeed(m_loadSpeed);
	m_pressurePlate->SetPressure(false);
	
	if (m_loaderData->IsLoadingComplete())
	{
		m_loaderData->SetLoad(false);
		m_collectorRollers->SetRunning(false);
		m_pressurePlate->SetPressure(true);
		return true;
	}
	m_ticks++;
	
	return false;
}

bool LoadLauncher::Abort()
{
	m_loaderData->SetLoad(false);
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(false);
	m_pressurePlate->SetPressure(true);
	return true;
}

void LoadLauncher::Configure()
{
	m_loadSpeed = GetConfig("load_speed", 1.0);
	m_upTicks = GetConfig("collector_up_ticks", 25);
}
