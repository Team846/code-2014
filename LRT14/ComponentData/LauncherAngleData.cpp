#include "LauncherAngleData.h"

LauncherAngleData::LauncherAngleData() :
	ComponentData("LauncherAngleData")
{
	
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
