#include "LauncherAngleData.h"

LauncherAngleData::LauncherAngleData() :
	ComponentData("LauncherAngleData")
{
	ResetCommands();
	m_complete = false;
	m_launcherAngle = LONG;
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
	LogToFile(&m_launcherAngle, "Angle");
	LogToFile(&m_complete, "Complete");
}

LauncherAngleData::Angle LauncherAngleData::GetAngle()
{
	return m_launcherAngle;
}

void LauncherAngleData::SetAngle(LauncherAngleData::Angle angle)
{
	m_launcherAngle = angle;
}

bool LauncherAngleData::IsCompleteState()
{
	return m_complete;
}

void LauncherAngleData::SetCompleteState(bool complete)
{
	m_complete = complete;
}
