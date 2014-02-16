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
	m_safety = new Pneumatics(ConfigPortMappings::Get("Solenoid/LAUNCHER_SAFETY"), "LauncherSafety");
	m_sensor = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/LAUNCHER_LOADER_SENSOR"));
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_LAUNCHER_PROXIMITY"));
	m_currentRotation = 0;
	m_desiredRotation = 0;
	m_desiredZero = 0;
	m_currentSensorValue = m_sensor->GetAverageValue();
	m_lastRawSensorValue = m_sensor->GetAverageValue();
	m_currentSetpoint = m_sensor->GetAverageValue();
	m_load = false;
}

LauncherLoader::~LauncherLoader()
{
	R_DELETE(m_motorA);
	R_DELETE(m_motorB);
}

void LauncherLoader::OnEnabled()
{
	
}

void LauncherLoader::OnDisabled()
{
	
}

void LauncherLoader::UpdateEnabled()
{
	int currentValue = m_sensor->GetAverageValue() - m_desiredZero;
	if (currentValue < 0)
		currentValue += m_maxSensorValue;
	
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
	if (m_loaderData->GetFire() && m_proximity->Get() == 0)
	{
		m_desiredRotation = m_currentRotation + 1;
		m_load = false;
	}
	else if (m_loaderData->GetLoad())
	{
		m_load = true;
	}
		
	if (m_loaderData->GetPurge())
	{
		m_currentSetpoint = m_unloadSetpoint + m_desiredRotation * m_maxSensorValue;
		m_load = false;
	}
	else
	{
		if (m_load)
		{
			m_currentSetpoint = m_loadSetpoint + m_desiredRotation * m_maxSensorValue;
		}
		else
		{
			m_currentSetpoint = m_intermediateSetpoint + m_desiredRotation * m_maxSensorValue;
		}
	}
	int error = m_currentSetpoint - currentValue;
	m_motorA->SetDutyCycle(m_gain * error);
	m_motorB->SetDutyCycle(m_gain * error);
	
	if (m_proximity->Get() == 1)
	{
		m_loaderData->SetBallDetected(false);
		m_safety->Set(Pneumatics::OFF);
	}
	else
	{
		m_loaderData->SetBallDetected(true);
		if (m_loaderData->GetFire())
			m_safety->Set(Pneumatics::FORWARD);
	}
}

void LauncherLoader::UpdateDisabled()
{
	m_motorA->SetDutyCycle(0.0);
	m_motorB->SetDutyCycle(0.0);
	m_safety->Set(Pneumatics::OFF);
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
	m_desiredZero = GetConfig("sensor_zero", 0);
}

void LauncherLoader::Send()
{
	SendToNetwork(m_currentSensorValue, "LoaderSensorValue", "RobotData");
}
