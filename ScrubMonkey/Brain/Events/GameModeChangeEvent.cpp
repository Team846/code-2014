#include "GameModeChangeEvent.h"

GameModeChangeEvent::GameModeChangeEvent(Rhesus::Toolkit::GameState::Enum toMode)
{
	m_toMode = toMode;
	m_from = false;
}

GameModeChangeEvent::GameModeChangeEvent(Rhesus::Toolkit::GameState::Enum fromMode, Rhesus::Toolkit::GameState::Enum toMode)
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
		if (m_from && RobotState::Instance().LastGameMode() != m_fromMode)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool GameModeChangeEvent::CheckCondition()
{
	return RobotState::Instance().GameMode() == m_toMode;
}

