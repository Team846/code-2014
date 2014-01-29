#include "LauncherAngleData.h"

LauncherAngleData::LauncherAngleData() :
	ComponentData("LauncherAngleData")
{
	ResetCommands();
}

LauncherAngleData* LauncherAngleData::Get()
{
	return (LauncherAngleData*) ComponentData::GetComponentData("LauncherAngleData");
}

void LauncherAngleData::ResetCommands()
{
	m_launcherAngle = LOW;
}

void LauncherAngleData::Log()
{
	LogToFile(&m_launcherAngle, "Angle");
}

LauncherAngleData::Angle LauncherAngleData::GetAngle()
{
	return m_launcherAngle;
}

void LauncherAngleData::SetAngle(LauncherAngleData::Angle angle)
{
	m_launcherAngle = angle;
}
