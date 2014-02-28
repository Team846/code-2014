#ifndef JOYSTICK_HELD_EVENT_H_
#define JOYSTICK_HELD_EVENT_H_

#include "DelayedEvent.h"
#include "../../DriverStation/DebouncedJoystick.h"
#include "JoystickPressedEvent.h"

/*!
 * @brief Event that fires when a joystick button is held.
 */
class JoystickHeldEvent : public DelayedEvent
{
public:
	JoystickHeldEvent(DebouncedJoystick* joystick, int button, int cycles);
	virtual ~JoystickHeldEvent();
	
	int GetButton();
	DebouncedJoystick* GetJoystick();
	
private:
	DebouncedJoystick* m_joystick;
	int m_lastFiredButton;
};

#endif
