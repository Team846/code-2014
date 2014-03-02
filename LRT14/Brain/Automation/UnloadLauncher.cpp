#include "UnloadLauncher.h"
#include "../../Config/ConfigPortMappings.h"

UnloadLauncher::UnloadLauncher() :
	Automation("UnloadLauncher", true),
	Configurable("UnloadLauncher")
{
	m_collectorArm = CollectorArmData::Get();
	m_loaderData = LauncherLoaderData::Get();
}

void UnloadLauncher::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::LAUNCHER_LOADER);
}

bool UnloadLauncher::Start()
{
	return true;
}

bool UnloadLauncher::Run()
{
	m_loaderData->SetPurge(true);
	if (m_loaderData->IsLoadingComplete())
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
		m_timer.Reset();
		m_timer.Start();
		if (m_timer.Get() > m_waitTime)
		{
			m_loaderData->SetPurge(false);
			return true;
		}
	}
	return false;
}

bool UnloadLauncher::Abort()
{
	return true;
}

void UnloadLauncher::Configure()
{
	m_waitTime = GetConfig("collector_down_wait", 1.0);
}
