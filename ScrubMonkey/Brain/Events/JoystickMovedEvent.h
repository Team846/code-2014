#ifndef JOYSTICK_MOVED_EVENT_H_
#define JOYSTICK_MOVED_EVENT_H_

#include "Event.h"
#include "../../DriverStation/DebouncedJoystick.h"

/*!
 * @brief Event that fires when a joystick axis is moved.
 */
class JoystickMovedEvent : public Event
{
public:
	JoystickMovedEvent(DebouncedJoystick* joystick, int axis = 0, float sensitivity = 0.02);
	virtual ~JoystickMovedEvent();
	
	virtual bool CheckCondition();
	
	uint32_t GetAxis();
	DebouncedJoystick* GetJoystick();
	
private:
	DebouncedJoystick* m_joystick;
	uint32_t m_axis;
	float m_sensitivity;
	uint32_t m_lastFiredAxis;
};

#endif
