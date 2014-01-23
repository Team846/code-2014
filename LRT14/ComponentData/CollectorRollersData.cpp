#include "CollectorRollersData.h"

CollectorRollersData::CollectorRollersData() :
	ComponentData("CollectorRollerData")
{
	m_running = false;
	m_direction = FORWARD;
}

CollectorRollersData* CollectorRollersData::Get()
{
	return (CollectorRollersData*) ComponentData::GetComponentData("CollectorRollerData");
}

void CollectorRollersData::ResetCommands()
{
	
}

void CollectorRollersData::Log()
{
	
}

void CollectorRollersData::SetRunning(bool run)
{
	m_running = run;
}

bool CollectorRollersData::IsRunning()
{
	return m_running;
}

void CollectorRollersData::SetDirection(Direction direction)
{
	m_direction = direction;
}

CollectorRollersData::Direction CollectorRollersData::GetDirection()
{
	return m_direction;
}
