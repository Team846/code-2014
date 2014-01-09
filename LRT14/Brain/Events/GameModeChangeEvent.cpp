#include "GameModeChangeEvent.h"
#include "../../LRTRobot14.h"

GameModeChangeEvent::GameModeChangeEvent(RobotState::Enum toMode)
{
	m_toMode = toMode;
	m_from = false;
}

GameModeChangeEvent::GameModeChangeEvent(RobotState::Enum fromMode, RobotState::Enum toMode)
{
	m_toMode = toMode;
	m_fromMode = fromMode;
	m_from = true;
}
	
GameModeChangeEvent::~GameModeChangeEvent()
{
}

bool GameModeChangeEvent::Fired()
{
	if (Event::Fired())
	{
		if (m_from && LRTRobot14::Robot()->LastGameState() != m_fromMode)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool GameModeChangeEvent::CheckCondition()
{
	return LRTRobot14::Robot()->GameState() == m_toMode;
}

