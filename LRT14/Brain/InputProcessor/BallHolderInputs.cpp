#include "BallHolderInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"

BallHolderInputs::BallHolderInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_ballHolder = BallHolderData::Get();
	
	Dashboard2::AddTelemetryData("Ball Holder Input", (INT16)DashboardTelemetryID::BALL_HOLDER_INPUT, DashboardTelemetryType::STRING);
	
	RegisterResource(ControlResource::BALL_HOLDER);
}

void BallHolderInputs::Update()
{
	std::string state = "???";
	
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::UNLOAD_LAUNCHER)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::BALL_HOLDER)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::KISS_PASS))
	{
		m_ballHolder->SetHold(false);
		
		state = "PRESSED";
	}
	else
	{
		m_ballHolder->SetHold(true);
		
		state = "RELEASED";
	}
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::BALL_HOLDER_INPUT, state);
}
