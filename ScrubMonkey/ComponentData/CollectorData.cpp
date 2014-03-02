#include "CollectorData.h"

CollectorData::CollectorData() :
	ComponentData("CollectorData")
{
	ResetCommands();
}


void CollectorData::SetEnabled(bool state)
{
	m_state = state;
}

bool CollectorData::IsEnabled()
{
	return m_state;
}

void CollectorData::SetDirection(Direction dir)
{
	m_direction = dir;
}

CollectorData::Direction CollectorData::GetDirection()
{
	return m_direction;
}

void CollectorData::ResetCommands()
{
	m_state = false;
	m_direction = COLLECT;
}

CollectorData* CollectorData::Get()
{
	return (CollectorData*)ComponentData::GetComponentData("Collector");
}
