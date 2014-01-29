#include "CollectorRollersData.h"

CollectorRollersData::CollectorRollersData() :
	ComponentData("CollectorRollerData")
{
	ResetCommands();
}

CollectorRollersData* CollectorRollersData::Get()
{
	return (CollectorRollersData*) ComponentData::GetComponentData("CollectorRollersData");
}

void CollectorRollersData::ResetCommands()
{
	m_running = false;
	m_direction = FORWARD;
	m_speed = 0.0;
}

void CollectorRollersData::Log()
{
	LogToFile(&m_running, "Running");
	LogToFile(&m_direction, "Direction");
	LogToFile(&m_speed, "Speed");
}

void CollectorRollersData::SetRunning(bool run)
{
	m_running = run;
}

bool CollectorRollersData::IsRunning()
{
	return m_running;
}

void CollectorRollersData::SetSpeed(float speed)
{
	m_speed = speed;
}

float CollectorRollersData::GetSpeed()
{
	return m_speed;
}

void CollectorRollersData::SetDirection(CollectorRollersData::Direction direction)
{
	m_direction = direction;
}

CollectorRollersData::Direction CollectorRollersData::GetDirection()
{
	return m_direction;
}
