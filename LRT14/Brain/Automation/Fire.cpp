#include "Fire.h"
#include "../../Config/ConfigPortMappings.h"

Fire::Fire() :
	Automation("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_collectorArmData = CollectorArmData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::COLLECTOR_ARM);
}

bool Fire::Start()
{
	return true;
}

bool Fire::Run()
{
	if (m_proximity == 0)
	{
		m_loaderData->SetFire(false);
		m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
		return true;
	}
	
	m_collectorArmData->SetDesiredPosition(CollectorArmData::COLLECT);
//	if (m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT)
		m_loaderData->SetFire(true);
	
	return false;
}

bool Fire::Abort()
{
	m_loaderData->SetFire(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
	return true;
}
