#include "Fire.h"

Fire::Fire() :
	Automation("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_collectorArmData = CollectorArmData::Get();
	m_firing = false;
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::COLLECTOR_ARM);
}

bool Fire::Start()
{
	m_firing = false;
	return true;
}

bool Fire::Run()
{
//	if (!m_loaderData->IsBallDetected())
//	{
//		if (m_loaderData->IsLoadingComplete())
//			m_loaderData->SetPurge(true);
//		return false;
//	}
	
	m_collectorArmData->SetDesiredPosition(CollectorArmData::COLLECT);
//	if (m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT)
		m_loaderData->SetFire(true);
	
//	if (m_firing || m_loaderData->IsLoadingComplete() && m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT)
//	{
//		m_loaderData->SetFire(true);
//		m_firing = true;
//		if (!m_loaderData->IsBallDetected())
//		{
//			m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
//			return true;
//		}
//	}
	return false;
}

bool Fire::Abort()
{
	m_loaderData->SetFire(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
	m_firing = false;
	return true;
}
