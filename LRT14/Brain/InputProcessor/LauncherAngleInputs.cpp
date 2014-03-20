#include "LauncherAngleInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"

LauncherAngleInputs::LauncherAngleInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_angleData = LauncherAngleData::Get();
	Dashboard2::AddTelemetryData("Launcher Angle Input", (INT16)DashboardTelemetryID::LAUNCHER_ANGLE_INPUT, DashboardTelemetryType::STRING);
	RegisterResource(ControlResource::LAUNCHER_ANGLE);
}

void LauncherAngleInputs::Update()
{
	std::string state = "RELEASED";
	
	if (m_operator_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::LONG_SHOT))
	{
		m_angleData->SetAngle(LauncherAngleData::LONG);
		
		state = "LONG";
	}
	else if (m_operator_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::SHORT_SHOT))
	{
		m_angleData->SetAngle(LauncherAngleData::SHORT);
		
		state = "SHORT";
	}
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::LAUNCHER_ANGLE_INPUT, state);
}
