#include "BallPusherData.h"

BallPusherData::BallPusherData() :
	ComponentData("BallPusherData")
{
	ResetCommands();
	m_push = false;
}

BallPusherData* BallPusherData::Get()
{
	return (BallPusherData*) ComponentData::GetComponentData("BallPusherData");
}

void BallPusherData::ResetCommands()
{
}

void BallPusherData::Log()
{
	LogToFile(&m_push, "Push");
}

bool BallPusherData::GetPush()
{
	return m_push;
}

void BallPusherData::SetPush(bool push)
{
	m_push = push;
}
