#include "CollectorArmData.h"

CollectorArmData::CollectorArmData() :
	ComponentData("CollectorArmData")
{
	m_position = CollectorArmData::STOWED;
}

void CollectorArmData::ResetCommands()
{
	
}

void CollectorArmData::Log()
{
	
}

void CollectorArmData::SetPosition(Position position)
{
	m_position = position;
}

CollectorArmData::Position CollectorArmData::GetPosition()
{
	return m_position;
}

CollectorArmData* CollectorArmData::Get()
{
	return (CollectorArmData*) ComponentData::GetComponentData("CollectorArmData");
}
