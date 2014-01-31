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
	LogToFile(&m_complete, "Complete");
}

void LauncherLoaderData::SetFire(bool shouldFire)
{
	m_fire = shouldFire;
}

bool LauncherLoaderData::GetFire()
{
	return m_fire;
}

bool LauncherLoaderData::IsLoadingComplete()
{
	return m_complete;
}

void LauncherLoaderData::SetLoadingComplete(bool complete)
{
	m_complete = complete;
}
