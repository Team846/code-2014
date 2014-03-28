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
	m_firing = false;
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
	m_collectorDownTimer.Reset();
	m_collectorDownTimer.Start();
	m_hasBall = false;
	m_firing = false;
	return true;
}

bool Fire::Run()
{
	m_pressurePlate->SetPressure(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::COLLECT);
	
	if (m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT || m_collectorDownTimer.Get() >= m_timeout)
	{
		m_loaderData->SetFire(true);
		if (!m_firing)
		{
			m_fireTimer.Reset();
			m_fireTimer.Start();
		}
		m_firing = true;
		if (m_fireTimer.Get() >= m_fireTime)
		{
			printf("Fire done\n");
			m_loaderData->SetFire(false);
			return true;
		}
	}
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
	m_fireTime = GetConfig("fire_time", 2.0);
}
