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
	LauncherAngleData::Angle angle = LauncherAngleData::SHORT;
	
	switch(m_launcherAngleData->GetAngle())
	{
	case LauncherAngleData::SHORT:
		state = Pneumatics::OFF;
		break;
	case LauncherAngleData::LONG:
		state = Pneumatics::FORWARD;
		break;
	default:
		m_pneumatics->Set(Pneumatics::OFF);
	}
	m_pneumatics->Set(state);
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE_STATE, (INT8)state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE, (INT8)angle);
}

void LauncherAngle::UpdateDisabled()
{
	Pneumatics::State state = Pneumatics::OFF;
	LauncherAngleData::Angle angle = m_launcherAngleData->GetAngle();
	
	m_pneumatics->Set(Pneumatics::OFF);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE_STATE, (INT8)state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE, (INT8)angle);
}

void LauncherAngle::Configure()
{
	
}
