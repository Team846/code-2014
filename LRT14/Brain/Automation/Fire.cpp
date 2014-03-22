#include "Fire.h"
#include "../../Config/ConfigPortMappings.h"

Fire::Fire() :
	Automation("Fire"),
	Configurable("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_collectorArmData = CollectorArmData::Get();
	m_pressurePlate = PressurePlateData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
	m_hasBall = false;
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::PRESSURE_PLATE);
}

bool Fire::Start()
{
	printf("Fire\n");
	m_timer.Reset();
	m_timer.Start();
	m_hasBall = false;
	return true;
}

bool Fire::Run()
{
	m_pressurePlate->SetPressure(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::COLLECT);
	
	if (m_proximity->Get() == 1)
	{
		m_hasBall = true;
	}
	else
	{
		m_loaderData->SetFire(false);
		if (m_hasBall)
		{
			m_pressurePlate->SetPressure(true);
			m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
		}
		return m_hasBall;
	}
	
	if (m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT || m_timer.Get() >= m_timeout)
		m_loaderData->SetFire(true);

	return false;
}

bool Fire::Abort()
{
	m_loaderData->SetFire(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
	m_pressurePlate->SetPressure(true);
	return true;
}

void Fire::Configure()
{
	m_timeout = GetConfig("timeout", 2.0);
}
