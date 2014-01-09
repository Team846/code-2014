#ifndef GAME_MODE_CHANGE_EVENT_H_
#define GAME_MODE_CHANGE_EVENT_H_

#include "Event.h"
#include "../../Rhesus/Toolkit/RobotState.h"

using namespace Rhesus::Toolkit;

/*!
 * @brief Event that fires when the game mode changes to a particular mode.
 */
class GameModeChangeEvent : public Event
{
public:
	GameModeChangeEvent(RobotState::Enum toMode);
	GameModeChangeEvent(RobotState::Enum fromMode, RobotState::Enum toMode);
	virtual ~GameModeChangeEvent();
	
	virtual bool Fired();
	virtual bool CheckCondition();
	
private:
	RobotState::Enum m_toMode;
	RobotState::Enum m_fromMode;
	bool m_from;
};

#endif
