#include "Fire.h"

Fire::Fire() :
	Automation("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_angleData = LauncherAngleData::Get();
	m_collectorArmData = CollectorArmData::Get();
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::LAUNCHER_ANGLE);
	AllocateResource(ControlResource::COLLECTOR_ARM);
}

bool Fire::Start()
{
	return true;
}

bool Fire::Run()
{
	m_collectorArmData->SetPosition(CollectorArmData::COLLECT);
	if (m_angleData->IsCompleteState() && m_loaderData->IsLoadingComplete())
	{
		m_loaderData->SetFire(true);
		return true;
	}
	else
		return false;
}

bool Fire::Abort()
{
	m_loaderData->SetFire(false);
	return true;
}

