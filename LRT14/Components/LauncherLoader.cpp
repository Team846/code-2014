#include "LauncherLoader.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"

LauncherLoader::LauncherLoader() : 
	Component("LauncherLoader", DriverStationConfig::DigitalIns::LAUNCHER_LOADER),
	Configurable("LauncherLoader")
{
	m_loaderData = LauncherLoaderData::Get();
	m_motorA = new LRTVictor(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_A"), "LauncherLoaderA");
	m_motorB = new LRTVictor(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_B"), "LauncherLoaderB");
	m_pneumatics = new Pneumatics(ConfigPortMappings::Get("Solenoid/LAUNCHER_SAFETY"), "LauncherSafety");
	m_sensor = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/LAUNCHER_LOADER_SENSOR"));
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_LAUNCHER_PROXIMITY"));
	m_currentRotation = 0;
	m_currentSensorValue = m_sensor->GetAverageValue();
	m_lastRawSensorValue = m_sensor->GetAverageValue();
	m_currentSetpoint = m_sensor->GetAverageValue();
	m_loaded = false;
}

LauncherLoader::~LauncherLoader()
{
	DELETE(m_motorA);
	DELETE(m_motorB);
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
	
	if (m_proximity->Get() == 1)
	{
		m_loaderData->SetBallDetected(false);
		m_pneumatics->Set(Pneumatics::OFF);
	}
	else
	{
		m_loaderData->SetBallDetected(true);
		if (m_loaderData->GetFire())
			m_pneumatics->Set(Pneumatics::FORWARD);
	}
}

void LauncherLoader::UpdateDisabled()
{
	m_motorA->SetDutyCycle(0.0);
	m_motorB->SetDutyCycle(0.0);
	m_pneumatics->Set(Pneumatics::OFF);
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
	SendToNetwork(m_currentSensorValue, "LoaderSensorValue", "RobotData");
}
