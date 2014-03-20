#include "LauncherAngle.h"

#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/Pneumatics.h"

LauncherAngle::LauncherAngle() :
	Component("LauncherAngle", DriverStationConfig::DigitalIns::LAUNCHER_ANGLE),
	Configurable("LauncherAngle")
{
	m_launcherAngleData = LauncherAngleData::Get();
	m_pneumatics = new Pneumatics(ConfigPortMappings::Get("Solenoid/LAUNCHER_ANGLE"), "LauncherAngle");
}

LauncherAngle::~LauncherAngle()
{

}

void LauncherAngle::OnEnabled()
{
	
}
void LauncherAngle::OnDisabled()
{
	
}
		
void LauncherAngle::UpdateEnabled()
{
	Pneumatics::State state = Pneumatics::OFF;
	std::string pneumaticsStateStr = "OFF";
	std::string angleStr = "SHORT";
	
	switch(m_launcherAngleData->GetAngle())
	{
	case LauncherAngleData::SHORT:
		state = Pneumatics::OFF;
		pneumaticsStateStr = "OFF";
		angleStr = "SHORT";
		break;
	case LauncherAngleData::LONG:
		state = Pneumatics::FORWARD;
		pneumaticsStateStr = "FORWARD";
		angleStr = "LONG";
		break;
	default:
		m_pneumatics->Set(Pneumatics::OFF);
		angleStr = "???";
		pneumaticsStateStr = "OFF";
		break;
	}
	m_pneumatics->Set(state);
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE_STATE, pneumaticsStateStr);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE, angleStr);
}

void LauncherAngle::UpdateDisabled()
{
	std::string pneumaticsStateStr = "OFF";
	LauncherAngleData::Angle angle = m_launcherAngleData->GetAngle();
	std::string angleStr = "???";
	
	switch(angle)
	{
	case LauncherAngleData::SHORT:
		angleStr = "SHORT";
		break;
	case LauncherAngleData::LONG:
		angleStr = "LONG";
		break;
	default:
		angleStr = "???";
		break;
	}
	
	m_pneumatics->Set(Pneumatics::OFF);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE_STATE, pneumaticsStateStr);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE, angleStr);
}

void LauncherAngle::Configure()
{
	
}
