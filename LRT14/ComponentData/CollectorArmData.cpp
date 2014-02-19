#include "CollectorArmData.h"

CollectorArmData::CollectorArmData() :
	ComponentData("CollectorArmData")
{
	ResetCommands();
	m_position = STOWED;
}

CollectorArmData* CollectorArmData::Get()
{
	return (CollectorArmData*) ComponentData::GetComponentData("CollectorArmData");
}

void CollectorArmData::ResetCommands()
{
}

void CollectorArmData::Log()
{
	LogToFile(&m_position, "Position");
}

void CollectorArmData::SetDesiredPosition(Position position)
{
	m_position = position;
}

CollectorArmData::Position CollectorArmData::GetDesiredPosition()
{
	return m_position;
}


void CollectorArmData::SetCurrentPosition(Position position)
{
	m_currentPosition = position;
}

CollectorArmData::Position CollectorArmData::GetCurrentPosition()
{
	return m_currentPosition;
}
