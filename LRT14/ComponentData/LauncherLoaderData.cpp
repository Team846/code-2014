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
	LogToFile(&m_purge, "Purge");
	LogToFile(&m_load, "Load");
	LogToFile(&m_complete, "Complete");
	LogToFile(&m_value, "SensorValue");
}

void LauncherLoaderData::SetFire(bool shouldFire)
{
	m_fire = shouldFire;
}

bool LauncherLoaderData::GetFire()
{
	return m_fire;
}

void LauncherLoaderData::SetPurge(bool purge)
{
	m_purge = purge;
}

bool LauncherLoaderData::GetPurge()
{
	return m_purge;
}

void LauncherLoaderData::SetLoad(bool load)
{
	m_load = load;
}

bool LauncherLoaderData::GetLoad()
{
	return m_load;
}

bool LauncherLoaderData::IsLoadingComplete()
{
	return m_complete;
}

void LauncherLoaderData::SetLoadingComplete(bool complete)
{
	m_complete = complete;
}

int LauncherLoaderData::GetSensorValue()
{
	return m_value;
}

void LauncherLoaderData::SetSensorValue(int value)
{
	m_value = value;
}
