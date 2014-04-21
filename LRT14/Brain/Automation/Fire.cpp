#include "Fire.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../DriverStation/LRTDriverStation.h"

Fire::Fire(bool collectorDown, bool overrideSensor) :
	Automation("Fire"),
	Configurable("Fire")
{
	m_loaderData = LauncherLoaderData::Get();
	m_collectorArmData = CollectorArmData::Get();
	m_ballHolder = BallHolderData::Get();
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_LAUNCHER_PROXIMITY"));
//	m_proximity = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/BALL_LAUNCHER_PROXIMITY"));
	m_hasBall = false;
	m_firing = false;
	m_override = overrideSensor;
	m_collectorDown = collectorDown;
}

void Fire::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::BALL_HOLDER);
}

bool Fire::Start()
{
	printf("Fire\n");
	m_collectorDownTimer.Reset();
	m_collectorDownTimer.Start();
	m_hasBall = false;
	m_firing = false;
	m_loaded = false;
	return true;
}

bool Fire::Run()
{
	m_ballHolder->SetHold(false);
	m_collectorArmData->SetDesiredPosition(m_collectorDown ? CollectorArmData::COLLECT : CollectorArmData::INTERMEDIATE);
	
	if (!m_loaderData->IsLoadingComplete() && !m_loaded)
	{
		m_loaderData->SetLoad(true);
		return false;
	}
	else
		m_loaded = true;
	
	if (/*m_proximity->GetAverageValue() >= m_detection*/m_proximity->Get() == 0 && !m_override && !m_firing && !LRTDriverStation::Instance()->GetOperatorStick()->IsButtonDown(DriverStationConfig::JoystickButtons::OVERRIDE_FIRE))
		return false;
	
//	if (m_collectorArmData->GetCurrentPosition() == CollectorArmData::COLLECT || m_collectorDownTimer.Get() >= m_timeout)
//	{
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
			m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
			m_ballHolder->SetHold(true);
			return true;
		}
//	}
	return false;
}

bool Fire::Abort()
{
	m_loaderData->SetFire(false);
	m_collectorArmData->SetDesiredPosition(CollectorArmData::STOWED);
	m_ballHolder->SetHold(true);
	return true;
}

void Fire::Configure()
{
	m_timeout = GetConfig("timeout", 2.0);
	m_fireTime = GetConfig("fire_time", 2.0);
	m_detection = GetConfig("detection_threshold", 0);
}
