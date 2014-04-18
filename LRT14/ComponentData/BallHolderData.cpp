#include "BallHolderData.h"

BallHolderData::BallHolderData() :
	ComponentData("BallHolderData")
{
	ResetCommands();
	m_hold = true;
}

BallHolderData* BallHolderData::Get()
{
	return (BallHolderData*) ComponentData::GetComponentData("BallHolderData");
}

void BallHolderData::ResetCommands()
{
}

void BallHolderData::Log()
{
	LogToFile(&m_hold, "Hold");
}

bool BallHolderData::GetHold()
{
	return m_hold;
}

void BallHolderData::SetHold(bool hold)
{
	m_hold = hold;
}
