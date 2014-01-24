#include "LauncherLoader.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"

LauncherLoader::LauncherLoader() : 
	Component("LauncherLoader", DriverStationConfig::DigitalIns::LAUNCHER_LOADER),
	Configurable("LauncherLoader")
{
	
	m_loaderData = LauncherLoaderData::Get();
	m_talon = new LRTTalon(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER"), "launcherLoader");
	m_sensor = new DigitalInput(ConfigPortMappings::Get("Digital/LAUNCHER_lOADER_SENSOR"));
	m_firing = false;
}

LauncherLoader::~LauncherLoader()
{
	DELETE(m_talon);
	DELETE(m_sensor);
}

void LauncherLoader::OnEnabled()
{
	
}

void LauncherLoader::OnDisabled()
{
	
}

void LauncherLoader::UpdateEnabled()
{
	if(m_sensor->Get() == 0)
	{
		if (m_loaderData->GetFire() || m_firing)
		{
			m_firing = true;
			m_talon->SetDutyCycle(m_speed);
		}
		else
			m_talon->SetDutyCycle(0.0);
	}
	else
	{
		m_firing = false;
		m_talon->SetDutyCycle(m_speed);
	}
}

void LauncherLoader::UpdateDisabled()
{
	
}

void LauncherLoader::Configure()
{
	m_speed = GetConfig("speed", 1.0);
}
