#include "LauncherLoader.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"

#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;

LauncherLoader::LauncherLoader() : 
	Component("LauncherLoader", DriverStationConfig::DigitalIns::LAUNCHER_LOADER),
	Configurable("LauncherLoader")
{
	m_loaderData = LauncherLoaderData::Get();
	m_motorA = new LRTVictor(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_A"), "LauncherLoaderA");
	m_motorB = new LRTVictor(ConfigPortMappings::Get("PWM/LAUNCHER_LOADER_B"), "LauncherLoaderB");
	m_safety = new Pneumatics(ConfigPortMappings::Get("Solenoid/LAUNCHER_SAFETY"), "LauncherSafety");
	m_sensor = new ContinuousPotentiometer(ConfigPortMappings::Get("Analog/LAUNCHER_LOADER_SENSOR_A"), ConfigPortMappings::Get("Analog/LAUNCHER_LOADER_SENSOR_B"));
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_LAUNCHER_PROXIMITY"));
	m_currentRotation = 0;
	m_desiredRotation = 0;
	m_desiredZero = 0;
	m_currentSensorAngle = m_sensor->GetAngle();
	m_lastRawSensorAngle = m_sensor->GetAngle();
	m_currentSetpoint = m_sensor->GetAngle();
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
	float currentAngle = 360 - m_sensor->GetAngle() - m_desiredZero;
	if (currentAngle < 0)
		currentAngle += 360;
	
	if (currentAngle < m_wrapThreshold && m_lastRawSensorAngle > 360 - m_wrapThreshold)
	{
		m_currentRotation++;
	}
	else if (currentAngle > 360 - m_wrapThreshold && m_lastRawSensorAngle < m_wrapThreshold)
	{
		m_currentRotation--;
	}
	m_lastRawSensorAngle = currentAngle;
	m_currentSensorAngle = m_currentRotation * 360 + currentAngle;
	m_loaderData->SetSensorValue(m_currentSensorAngle);
	if (m_loaderData->GetLoad())
	{
		m_load = true;
	}
	if (m_loaderData->GetFire() && m_proximity->Get() == 0)
	{
		m_desiredRotation = m_currentRotation + 1;
		m_load = false;
	}
	else if (m_loaderData->GetPurge())
	{
		m_currentSetpoint = m_unloadSetpoint + m_desiredRotation * 360;
		if (m_currentSensorAngle < m_intermediateSetpoint + m_desiredRotation * 360)
			m_load = false;
	}
	else
	{
		if (m_load)
		{
			m_currentSetpoint = m_loadSetpoint + m_desiredRotation * 360;
		}
		else
		{
			m_currentSetpoint = m_intermediateSetpoint + m_desiredRotation * 360;
		}
	}
	float error = m_currentSetpoint - currentAngle;
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
	m_unloadSetpoint = GetConfig("unload_setpoint", 0.0);
	m_intermediateSetpoint = GetConfig("intermediate_setpoint", 180.0);
	m_loadSetpoint = GetConfig("load_setpoint", 340.0);
	m_gain = GetConfig("gain", 1.0);
	m_wrapThreshold = GetConfig("wrap_threshold", 20.0);
	m_completionErrorThreshold = GetConfig("completion_error_threshold", 5.0);
	m_desiredZero = GetConfig("sensor_zero", 0.0);
}

void LauncherLoader::Send()
{
	SendToNetwork(m_sensor->GetAngle(), "LoaderSensorRawAngle", "RobotData");
	SendToNetwork(m_currentSensorAngle, "LoaderSensorAngle", "RobotData");
}
