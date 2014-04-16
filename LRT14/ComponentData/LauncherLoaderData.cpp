#include "LauncherLoaderData.h"

LauncherLoaderData::LauncherLoaderData() :
	ComponentData("LauncherLoaderData")
{
	ResetCommands();
	m_complete = false;
	m_value = 0;
	m_ballDetected = false;
	m_fire = false;
	m_unload = false;
	m_load = false;
	m_trigger = false;
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
	LogToFile(&m_fire, "Fire");
	LogToFile(&m_unload, "Purge");
	LogToFile(&m_load, "Load");
	LogToFile(&m_complete, "Complete");
	LogToFile(&m_value, "SensorValue");
	LogToFile(&m_ballDetected, "BallDetected");
}

void LauncherLoaderData::SetFire(bool shouldFire)
{
	if (!m_fire && shouldFire)
		m_complete = false;
	m_fire = shouldFire;
}

bool LauncherLoaderData::GetFire()
{
	return m_fire;
}

void LauncherLoaderData::SetUnload(bool unload)
{
	if (!m_unload && unload)
		m_complete = false;
	m_unload = unload;
}

bool LauncherLoaderData::GetUnload()
{
	return m_unload;
}

void LauncherLoaderData::SetLoad(bool load)
{
	if (!m_load && load)
		m_complete = false;
	m_load = load;
}

bool LauncherLoaderData::GetLoad()
{
	return m_load;
}

void LauncherLoaderData::SetHairTrigger(bool trigger)
{
	if (!m_trigger && trigger)
		m_complete = false;
	m_trigger = trigger;
}

bool LauncherLoaderData::GetHairTrigger()
{
	return m_trigger;
}

void LauncherLoaderData::SetPurge(bool purge)
{
	if (!m_purge && purge)
		m_complete = false;
	m_purge = purge;
}

bool LauncherLoaderData::GetPurge()
{
	return m_purge;
}

bool LauncherLoaderData::IsLoadingComplete()
{
	return m_complete;
}

void LauncherLoaderData::SetLoadingComplete(bool complete)
{
	m_complete = complete;
}

float LauncherLoaderData::GetSensorValue()
{
	return m_value;
}

void LauncherLoaderData::SetSensorValue(float value)
{
	m_value = value;
}

bool LauncherLoaderData::IsBallDetected()
{
	return m_ballDetected;
}

void LauncherLoaderData::SetBallDetected(bool detected)
{
	m_ballDetected = detected;
}
