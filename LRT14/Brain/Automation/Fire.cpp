#include "Fire.h"

Fire::Fire() :
	Automation("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_angleData = LauncherAngleData::Get();
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::LAUNCHER_ANGLE);
}

bool Fire::Start()
{
	return true;
}

bool Fire::Run()
{
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
	return true;
}

