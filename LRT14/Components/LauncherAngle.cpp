#include "LauncherAngle.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"
#include "../Utils/Util.h"

LauncherAngle::LauncherAngle() :
	Component("LauncherAngle", DriverStationConfig::DigitalIns::LAUNCHER_ANGLE),
	Configurable("LauncherAngle")
{
	m_launcherAngleData = LauncherAngleData::Get();
	m_talon = new LRTTalon(ConfigPortMappings::Get("PWM/LAUNCHER_ANGLE"), "LauncherAngle");
	analogChannel = new AnalogChannel(ConfigPortMappings::Get("Analog/LAUNCHER_ANGLE"));
}

LauncherAngle::~LauncherAngle()
{
	delete m_talon;
	m_talon = NULL;
}

void LauncherAngle::OnEnabled()
{
	
}
void LauncherAngle::OnDisabled()
{
	
}
		
void LauncherAngle::UpdateEnabled()
{
	float setpoint;
	if (m_launcherAngleData->GetAngle() == LauncherAngleData::LOW)
		setpoint = m_highSetpoint;
	else if (m_launcherAngleData->GetAngle() == LauncherAngleData::HIGH)
		setpoint = m_lowSetpoint;
	
	float error = setpoint - analogChannel->GetAverageValue();
	m_talon->SetDutyCycle(error * m_gain);
}

void LauncherAngle::UpdateDisabled()
{
	
}
void LauncherAngle::Configure()
{
	m_lowSetpoint = GetConfig("low_setpoint", 1.0);
	m_highSetpoint = GetConfig("high_setpoint", 1.0);
	m_gain = GetConfig("gain", 1.0);
}
