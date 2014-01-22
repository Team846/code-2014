#include "CollectorRollerData.h"

CollectorRollerData::CollectorRollerData() :
	ComponentData("CollectorRollerData")
{
	m_enabled = false;
	m_direction = CollectorRollerData::FORWARD;
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

void CollectorRollerData::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool CollectorRollerData::GetEnabled()
{
	return m_enabled;
}

void CollectorRollerData::SetDirection(RollerDirection r)
{
	m_direction = r;
}

CollectorRollerData::RollerDirection CollectorRollerData::GetDirection()
{
	return m_direction;
}
