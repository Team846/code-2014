#ifndef TURN_JOYSTICK_DIRECTION_H_
#define TURN_JOYSTICK_DIRECTION_H_

#include "Automation.h"
#include "Turn.h"

#include "../../Config/Configurable.h"

class TurnJoystickDirection : public Turn
{
public:
	TurnJoystickDirection(double turnDegrees, double maxSpeed = 1.0);
	
	bool Start();
	
private:
	double m_turnDegrees;
};

#endif
