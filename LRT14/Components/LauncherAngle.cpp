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
	int setpoint;
	if (m_launcherAngleData->GetAngle() == LauncherAngleData::LONG)
		setpoint = m_longSetpoint;
	else if (m_launcherAngleData->GetAngle() == LauncherAngleData::SHORT)
		setpoint = m_shortSetpoint;
	
	int error = setpoint - analogChannel->GetAverageValue();
	m_talon->SetDutyCycle(error * m_gain);
	
	if (fabs(error) <= m_completionErrorThreshold)
		m_launcherAngleData->SetCompleteState(true);
	else
		m_launcherAngleData->SetCompleteState(false);
}

void LauncherAngle::UpdateDisabled()
{
	m_talon->SetDutyCycle(0.0);
}
void LauncherAngle::Configure()
{
	m_shortSetpoint = GetConfig("short_setpoint", 0);
	m_longSetpoint = GetConfig("long_setpoint", 0);
	m_gain = GetConfig("gain", 1.0);
	m_completionErrorThreshold = GetConfig("completion_error_threshold", 5);
}
