#include "LauncherLoaderData.h"

LauncherLoaderData::LauncherLoaderData() :
	ComponentData("LauncherLoaderData")
{
	ResetCommands();
}

LauncherLoaderData* LauncherLoaderData::Get()
{
	return (LauncherLoaderData*) ComponentData::GetComponentData("LauncherLoaderData");
}

void LauncherLoaderData::ResetCommands()
{
	m_fire = false;
}

void LauncherLoaderData::Log()
{
	LogToFile(&m_fire, "Fire");
}

void LauncherLoaderData::SetFire(bool shouldFire)
{
	m_fire = shouldFire;
}

bool LauncherLoaderData::GetFire()
{
	return m_fire;
}
