#include "TurnJoystickDirection.h"
#include "../../DriverStation/LRTDriverStation.h"
#include "../../Rhesus/Toolkit/Utilities/MathUtils.h"

TurnJoystickDirection::TurnJoystickDirection(double turnDegrees, double maxSpeed) :
	Turn(0.0, maxSpeed),
	m_turnDegrees(turnDegrees)
{
}

bool TurnJoystickDirection::Start()
{
	int dir = -Rhesus::Toolkit::Utilities::MathUtils::Sign(LRTDriverStation::Instance()->GetDriverStick()->GetAxis(Joystick::kZAxis));
	setAngle(m_turnDegrees * dir);
	return Turn::Start();
}
