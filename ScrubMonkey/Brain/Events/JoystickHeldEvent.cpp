#include "JoystickHeldEvent.h"

JoystickHeldEvent::JoystickHeldEvent(DebouncedJoystick* joystick, int button, int cycles) :
	DelayedEvent(new JoystickPressedEvent(joystick, button), cycles)
{
	m_joystick = joystick;
	m_lastFiredButton = button;
}

JoystickHeldEvent::~JoystickHeldEvent()
{
}

int JoystickHeldEvent::GetButton()
{
	return m_lastFiredButton;
}

DebouncedJoystick* JoystickHeldEvent::GetJoystick()
{
	return m_joystick;
}
