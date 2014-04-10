#include "LauncherLoader.h"

#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
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
	m_currentRotation = 0;
	m_desiredRotation = 0;
	m_desiredZero = 0;
	Configure();
	m_currentSensorAngle = 360 - m_sensor->GetAngle() - m_desiredZero;
	if (m_currentSensorAngle < 0)
		m_currentSensorAngle += 360;
	m_lastRawSensorAngle = 360 - m_sensor->GetAngle() - m_desiredZero;
	if (m_lastRawSensorAngle < 0)
		m_lastRawSensorAngle += 360;
	m_currentSetpoint = 360 - m_sensor->GetAngle() - m_desiredZero;
	if (m_currentSetpoint < 0)
		m_currentSetpoint += 360;
	m_load = false;
	m_lastFiring = false;
	m_maxSpeed = 1.0;
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
	UpdateSensorValues();
	Pneumatics::State state = Pneumatics::FORWARD;
	std::string stateStr = "FORWARD";
	
	m_safety->Set(state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_LOADER_SAFETY, stateStr);
	
	if (m_loaderData->GetFire())
	{
		BufferedConsole::Printf("Launcher Loader: Firing\n");
//		if (m_lastRawSensorAngle < m_firePoint || m_lastRawSensorAngle > m_firePoint + 10)
//		{
			m_motorA->SetDutyCycle(m_maxSpeed);
			m_motorB->SetDutyCycle(m_maxSpeed);
			m_loaderData->SetLoadingComplete(false);
//		}
//		else
//		{
//			m_motorA->SetDutyCycle(0.0);
//			m_motorB->SetDutyCycle(0.0);
//			m_loaderData->SetLoadingComplete(true);
//		}
	}
	else if (m_loaderData->GetLoad())
	{
		if (m_lastRawSensorAngle < m_loadSetpoint)
		{
			m_motorA->SetDutyCycle(m_maxSpeed);
			m_motorB->SetDutyCycle(m_maxSpeed);
			m_loaderData->SetLoadingComplete(false);
		}
		else
		{
			m_motorA->SetDutyCycle(0.0);
			m_motorB->SetDutyCycle(0.0);
			m_loaderData->SetLoadingComplete(true);
		}
	}
	else if (m_loaderData->GetHairTrigger())
	{
		if (m_lastRawSensorAngle < m_hairTriggerSetpoint)
		{
			m_motorA->SetDutyCycle(m_maxSpeed);
			m_motorB->SetDutyCycle(m_maxSpeed);
			m_loaderData->SetLoadingComplete(false);
		}
		else
		{
			m_motorA->SetDutyCycle(0.0);
			m_motorB->SetDutyCycle(0.0);
			m_loaderData->SetLoadingComplete(true);
		}
	}
	else if (m_loaderData->GetPurge())
	{
		if (m_lastRawSensorAngle > m_unloadSetpoint)
		{
			m_motorA->SetDutyCycle(-m_maxSpeed);
			m_motorB->SetDutyCycle(-m_maxSpeed);
			m_loaderData->SetLoadingComplete(false);
		}
		else
		{
			m_motorA->SetDutyCycle(0.0);
			m_motorB->SetDutyCycle(0.0);
			m_loaderData->SetLoadingComplete(true);
		}
	}
	else if (m_lastRawSensorAngle > 0.0 && m_lastRawSensorAngle < m_intermediateSetpoint)
	{
		m_motorA->SetDutyCycle(m_maxSpeed);
		m_motorB->SetDutyCycle(m_maxSpeed);
		m_loaderData->SetLoadingComplete(false);
	}
	else if (m_lastRawSensorAngle > m_loadSetpoint + 10)
	{
		m_motorA->SetDutyCycle(-m_maxSpeed);
		m_motorB->SetDutyCycle(-m_maxSpeed);
		m_loaderData->SetLoadingComplete(false);
	}
	else
	{
		m_motorA->SetDutyCycle(0.0);
		m_motorB->SetDutyCycle(0.0);
		m_loaderData->SetLoadingComplete(true);
	}
}

void LauncherLoader::UpdateDisabled()
{
	Pneumatics::State state = Pneumatics::OFF;
	std::string stateStr = "OFF";
			
	UpdateSensorValues();
	m_motorA->SetDutyCycle(0.0);
	m_motorB->SetDutyCycle(0.0);
	m_safety->Set(state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_LOADER_SAFETY, stateStr);
}

void LauncherLoader::UpdateSensorValues()
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
}

void LauncherLoader::Configure()
{
	m_unloadSetpoint = GetConfig("unload_setpoint", 5.0);
	m_intermediateSetpoint = GetConfig("intermediate_setpoint", 180.0);
	m_loadSetpoint = GetConfig("load_setpoint", 350.0);
	m_hairTriggerSetpoint = GetConfig("hair_trigger_setpoint", 360.0);
	m_firePoint = GetConfig("fire_point", 0.0);
	m_gain = GetConfig("gain", 1.0);
	m_wrapThreshold = GetConfig("wrap_threshold", 20.0);
	m_completionErrorThreshold = GetConfig("completion_error_threshold", 5.0);
	m_desiredZero = GetConfig("sensor_zero", 0.0);
	m_maxSpeed = GetConfig("max_speed", 1.0);
}

void LauncherLoader::Send()
{
	UpdateSensorValues();
	SendToNetwork(m_lastRawSensorAngle, "LoaderSensorRawAngle", "RobotData");
	SendToNetwork(m_currentSensorAngle / 360, "LoaderSensorAngle", "RobotData");
	SendToNetwork(m_currentRotation, "LoaderSensorRotation", "RobotData");
}
