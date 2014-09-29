#include "CollectorRollersInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"

CollectorRollersInputs::CollectorRollersInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_rollersData = CollectorRollersData::Get();
	Dashboard2::AddTelemetryData("Collector Rollers Input", (INT16)DashboardTelemetryID::COLLECTOR_ROLLERS_INPUT, DashboardTelemetryType::STRING);
	RegisterResource(ControlResource::COLLECTOR_ROLLERS);
}

void CollectorRollersInputs::Update()
{
	m_rollersData->SetRunning(false);
	
	std::string state = "RELEASED";
	
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::KISS_PASS)
			|| m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::DRIBBLE))
	{
		m_rollersData->SetRunning(true);
		m_rollersData->SetSpeed(1.0);
		m_rollersData->SetDirection(CollectorRollersData::FORWARD);
		
		state = "FORWARD";
	}
	else if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::REVERSE_ROLLERS))
	{
		m_rollersData->SetRunning(true);
		m_rollersData->SetSpeed(1.0);
		m_rollersData->SetDirection(CollectorRollersData::REVERSE);
		
		state = "REVERSE";
	}
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ROLLERS_INPUT, state);
}
