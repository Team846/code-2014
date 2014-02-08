#include "LauncherLoader.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"

LauncherLoader::LauncherLoader() : 
	Component("LauncherLoader", DriverStationConfig::DigitalIns::LAUNCHER_LOADER),
	Configurable("LauncherLoader")
{
	m_loaderData = LauncherLoaderData::Get();
	m_victorA = new LRTVictor(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_A"), "LauncherLoaderA");
	m_victorB = new LRTVictor(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_B"), "LauncherLoaderB");
	m_sensor = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/LAUNCHER_LOADER_SENSOR"));
	m_currentRotation = 0;
	m_currentSensorValue = m_sensor->GetAverageValue();
	m_lastRawSensorValue = m_sensor->GetAverageValue();
	m_currentSetpoint = m_sensor->GetAverageValue();
	m_loaded = false;
}

LauncherLoader::~LauncherLoader()
{
	DELETE(m_victorA);
	DELETE(m_victorB);
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
	int currentValue = m_sensor->GetAverageValue();
	if (currentValue < m_wrapThreshold && m_lastRawSensorValue > m_maxSensorValue - m_wrapThreshold)
	{
		m_currentRotation++;
	}
	else if (currentValue > m_maxSensorValue - m_wrapThreshold && m_lastRawSensorValue < m_wrapThreshold)
	{
		m_currentRotation--;
	}
	m_lastRawSensorValue = currentValue;
	m_currentSensorValue = m_currentRotation * m_maxSensorValue + currentValue;
	m_loaderData->SetSensorValue(m_currentSensorValue);
	if (m_loaderData->GetFire())
	{
		m_currentSetpoint = (m_currentRotation + 1) * m_maxSensorValue + m_intermediateSetpoint;
	}
	else if (m_loaderData->GetLoad())
	{
		m_currentSetpoint = m_currentRotation * m_maxSensorValue + m_loadSetpoint;
	}
	if (m_loaderData->GetPurge())
	{
		// ???
	}
	else
	{
		
	}
}

void LauncherLoader::UpdateDisabled()
{
	m_victorA->SetDutyCycle(0.0);
	m_victorB->SetDutyCycle(0.0);
}

void LauncherLoader::Configure()
{
	m_unloadSetpoint = GetConfig("unload_setpoint", 0);
	m_intermediateSetpoint = GetConfig("intermediate_setpoint", 800);
	m_loadSetpoint = GetConfig("load_setpoint", 1000);
	m_gain = GetConfig("gain", 1.0);
	m_wrapThreshold = GetConfig("wrap_threshold", 10);
	m_maxSensorValue = GetConfig("max_value", 1400);
	m_completionErrorThreshold = GetConfig("completion_error_threshold", 5);
}

void LauncherLoader::Send()
{
	SendToNetwork(m_currentSensorValue, "SensorValue", "RobotData");
}
