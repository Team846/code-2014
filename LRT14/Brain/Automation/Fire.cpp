#include "Fire.h"

Fire::Fire() :
	Automation("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_angleData = LauncherAngleData::Get();
	m_collectorArmData = CollectorArmData::Get();
	m_firing = false;
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::LAUNCHER_ANGLE);
	AllocateResource(ControlResource::COLLECTOR_ARM);
}

bool Fire::Start()
{
	m_firing = false;
	return true;
}

bool Fire::Run()
{
	if (!m_loaderData->IsBallDetected())
		return false;
	
	m_collectorArmData->SetDesiredPosition(CollectorArmData::COLLECT);
	if (m_firing || m_angleData->IsCompleteState() && m_loaderData->IsLoadingComplete() && m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT)
	{
		m_loaderData->SetFire(true);
		m_firing = true;
		if (!m_loaderData->IsBallDetected())
			return true;
	}
	return false;
}

bool Fire::Abort()
{
	m_loaderData->SetFire(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
	m_firing = false;
	return true;
}
