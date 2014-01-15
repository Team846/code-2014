#ifndef GAME_MODE_CHANGE_EVENT_H_
#define GAME_MODE_CHANGE_EVENT_H_

#include "Event.h"
#include "../../RobotState.h"

#include "../../Rhesus/Toolkit/GameState.h"

/*!
 * @brief Event that fires when the game mode changes to a particular mode.
 */
class GameModeChangeEvent : public Event
{
public:
	GameModeChangeEvent(Rhesus::Toolkit::GameState::Enum toMode);
	GameModeChangeEvent(Rhesus::Toolkit::GameState::Enum fromMode, Rhesus::Toolkit::GameState::Enum toMode);
	virtual ~GameModeChangeEvent();
	
	virtual bool Fired();
	virtual bool CheckCondition();
	
private:
	Rhesus::Toolkit::GameState::Enum m_toMode;
	Rhesus::Toolkit::GameState::Enum m_fromMode;
	bool m_from;
};

#endif
