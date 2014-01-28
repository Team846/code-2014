#include "LauncherLoader.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"

LauncherLoader::LauncherLoader() : 
	Component("LauncherLoader", DriverStationConfig::DigitalIns::LAUNCHER_LOADER),
	Configurable("LauncherLoader")
{
	m_loaderData = LauncherLoaderData::Get();
	m_talon = new LRTTalon(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER"), "LauncherLoader");
	m_sensor = new AnalogChannel(ConfigPortMappings::Get("Analog/LAUNCHER_LOADER_SENSOR"));
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
	if(m_loaderData->GetFire() || m_firing)
	{
		m_talon->SetDutyCycle(m_speed);
		m_firing = true;
		if(m_sensor->GetAverageValue() > m_closed_loop_threshold)
			m_firing = false;
	}
	else
	{
		float error = setpoint - m_sensor->GetAverageValue();
		m_talon->SetDutyCycle(error * m_gain);
	}
}

void LauncherLoader::UpdateDisabled()
{
	m_talon->SetDutyCycle(0.0);
}

void LauncherLoader::Configure()
{
	m_speed = GetConfig("speed", 1.0);
	setpoint = GetConfig("setpoint", 1.0);
	m_gain = GetConfig("gain", 1.0);
	m_closed_loop_threshold = GetConfig("closed_loop_threshold", 1.0);
}
