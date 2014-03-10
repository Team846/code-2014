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
	m_loadSpeed = 1.0;
	
}

void LoadLauncher::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
}

bool LoadLauncher::Start()
{
	return true;
}

bool LoadLauncher::Run()
{
	m_loaderData->SetLoad(true);
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(true);
	m_collectorRollers->SetDirection(CollectorRollersData::FORWARD);
	m_collectorRollers->SetSpeed(m_loadSpeed);
	
	if (m_loaderData->IsLoadingComplete())
		return true;
	
	return false;
}

bool LoadLauncher::Abort()
{
	m_loaderData->SetLoad(false);
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_collectorRollers->SetRunning(false);
	return true;
}

void LoadLauncher::Configure()
{
	m_loadSpeed = GetConfig("load_speed", 1.0);
}
