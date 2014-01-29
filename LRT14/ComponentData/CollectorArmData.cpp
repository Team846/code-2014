#include "CollectorArmData.h"

CollectorArmData::CollectorArmData() :
	ComponentData("CollectorArmData")
{
	ResetCommands();
}

CollectorArmData* CollectorArmData::Get()
{
	return (CollectorArmData*) ComponentData::GetComponentData("CollectorArmData");
}

void CollectorArmData::ResetCommands()
{
	m_position = STOWED;
}

void CollectorArmData::Log()
{
	LogToFile(&m_position, "Position");
}

void CollectorArmData::SetPosition(Position position)
{
	m_position = position;
}

CollectorArmData::Position CollectorArmData::GetPosition()
{
	return m_position;
}
