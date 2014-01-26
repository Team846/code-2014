#include "LauncherAngleData.h"

LauncherAngleData::LauncherAngleData() :
	ComponentData("LauncherAngleData")
{
	m_launcherAngle = LOW;
}

LauncherAngleData* LauncherAngleData::Get()
{
	return (LauncherAngleData*) ComponentData::GetComponentData("LauncherAngleData");
}

void LauncherAngleData::ResetCommands()
{
	
}

void LauncherAngleData::Log()
{
	

}

LauncherAngleData::Angle LauncherAngleData::GetAngle()
{
	return m_launcherAngle;
}

void LauncherAngleData::SetAngle(LauncherAngleData::Angle angle)
{
	m_launcherAngle = angle;
}
