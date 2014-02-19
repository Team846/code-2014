#include "LauncherAngle.h"

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
	switch(m_launcherAngleData->GetAngle())
	{
	case LauncherAngleData::SHORT:
		m_pneumatics->Set(Pneumatics::OFF);
		break;
	case LauncherAngleData::LONG:
		m_pneumatics->Set(Pneumatics::FORWARD);
		break;
	default:
		m_pneumatics->Set(Pneumatics::OFF);
	}
}

void LauncherAngle::UpdateDisabled()
{
	m_pneumatics->Set(Pneumatics::OFF);	
}

void LauncherAngle::Configure()
{
	
}
