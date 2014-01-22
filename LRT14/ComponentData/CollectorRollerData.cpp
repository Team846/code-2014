#include "CollectorRollerData.h"

CollectorRollerData::CollectorRollerData() :
	ComponentData("CollectorRollerData")
{
	m_running = false;
	m_direction = FORWARD;
}

CollectorRollerData* CollectorRollerData::Get()
{
	return (CollectorRollerData*) ComponentData::GetComponentData("CollectorRollerData");
}

void CollectorRollerData::ResetCommands()
{
	
}

void CollectorRollerData::Log()
{
	
}

void CollectorRollerData::SetRunning(bool run)
{
	m_running = run;
}

bool CollectorRollerData::IsRunning()
{
	return m_running;
}

void CollectorRollerData::SetDirection(Direction direction)
{
	m_direction = direction;
}

CollectorRollerData::Direction CollectorRollerData::GetDirection()
{
	return m_direction;
}
