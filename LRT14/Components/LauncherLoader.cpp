#include "LauncherLoader.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"

LauncherLoader::LauncherLoader() : 
	Component("LauncherLoader", DriverStationConfig::DigitalIns::LAUNCHER_LOADER),
	Configurable("LauncherLoader")
{
	m_loaderData = LauncherLoaderData::Get();
	m_talonA = new LRTTalon(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_A"), "LauncherLoaderA");
	m_talonB = new LRTTalon(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_B"), "LauncherLoaderB");
	m_sensor = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/LAUNCHER_LOADER_SENSOR"));
	m_firing = false;
}

LauncherLoader::~LauncherLoader()
{
	DELETE(m_talonA);
	DELETE(m_talonB);
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
	if (m_loaderData->GetFire() || m_firing)
	{
		m_talonA->SetDutyCycle(m_forwardSpeed);
		m_talonB->SetDutyCycle(m_forwardSpeed);
		m_firing = true;
		if(m_sensor->GetAverageValue() > m_closed_loop_threshold)
			m_firing = false;
		m_loaderData->SetLoadingComplete(false);
	}
	else if (m_loaderData->GetPurge())
	{
		m_talonA->SetDutyCycle(m_reverseSpeed);
		m_talonB->SetDutyCycle(m_reverseSpeed);
	}
	else
	{
		float error = setpoint - m_sensor->GetAverageValue();
		m_talonA->SetDutyCycle(error * m_gain);
		m_talonB->SetDutyCycle(error * m_gain);
		if (fabs(error) <= m_completionErrorThreshold)
			m_loaderData->SetLoadingComplete(true);
		else
			m_loaderData->SetLoadingComplete(false);
	}
}

void LauncherLoader::UpdateDisabled()
{
	m_talonA->SetDutyCycle(0.0);
	m_talonB->SetDutyCycle(0.0);
}

void LauncherLoader::Configure()
{
	m_forwardSpeed = GetConfig("speed", 1.0);
	m_reverseSpeed = GetConfig("purge_speed", 1.0);
	setpoint = GetConfig("setpoint", 1.0);
	m_gain = GetConfig("gain", 1.0);
	m_closed_loop_threshold = GetConfig("closed_loop_threshold", 1.0);
	m_completionErrorThreshold = GetConfig("completion_error_threshold", 5);
}
