#include "LauncherAngleInputs.h"
#include "../../Config/RobotConfig.h"

LauncherAngleInputs::LauncherAngleInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_angleData = LauncherAngleData::Get();
	RegisterResource(ControlResource::LAUNCHER_ANGLE);
}

void LauncherAngleInputs::Update()
{
	if (m_operator_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::LONG_SHOT))
	{
		m_angleData->SetAngle(LauncherAngleData::LONG);
	}
	else if (m_operator_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::SHORT_SHOT))
	{
		m_angleData->SetAngle(LauncherAngleData::SHORT);
	}
}
