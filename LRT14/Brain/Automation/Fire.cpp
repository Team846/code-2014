#include "Fire.h"
#include "../../Config/ConfigPortMappings.h"

Fire::Fire() :
	Automation("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_collectorArmData = CollectorArmData::Get();
	m_pressurePlate = PressurePlateData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::PRESSURE_PLATE);
}

bool Fire::Start()
{
	return true;
}

bool Fire::Run()
{
	if (m_proximity->Get() == 0)
	{
		m_loaderData->SetFire(false);
		m_pressurePlate->SetPressure(true);
		m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
		return true;
	}
	
	m_collectorArmData->SetDesiredPosition(CollectorArmData::COLLECT);
	if (m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT)
		m_loaderData->SetFire(true);

	m_pressurePlate->SetPressure(false);
	return false;
}

bool Fire::Abort()
{
	m_loaderData->SetFire(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
	m_pressurePlate->SetPressure(true);
	return true;
}
