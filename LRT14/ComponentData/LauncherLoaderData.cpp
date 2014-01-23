#include "LauncherLoaderData.h"

LauncherLoaderData::LauncherLoaderData() :
	ComponentData("LauncherLoaderData")
{
	
}

LauncherLoaderData* LauncherLoaderData::Get()
{
	return (LauncherLoaderData*) ComponentData::GetComponentData("LauncherLoaderData");
}

void LauncherLoaderData::ResetCommands()
{
	
}

void LauncherLoaderData::Log()
{
	
}
