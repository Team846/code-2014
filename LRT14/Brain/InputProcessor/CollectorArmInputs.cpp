#include "CollectorArmInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"

CollectorArmInputs::CollectorArmInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_armData = CollectorArmData::Get();
	Dashboard2::AddTelemetryData("Collector Arm Input", (INT16)DashboardTelemetryID::COLLECTOR_ARM_INPUT, DashboardTelemetryType::STRING);
	RegisterResource(ControlResource::COLLECTOR_ARM);
}

void CollectorArmInputs::Update()
{
	std::string telemState = "???";
	
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::OVERRIDE_FIRE))
	{
		m_armData->SetDesiredPosition(CollectorArmData::COLLECT);
		telemState = "PRESSED";
	}
	else if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::FIRE_PREPARE))
	{
		m_armData->SetDesiredPosition(CollectorArmData::INTERMEDIATE);
//		m_armData->SetDesiredPosition(CollectorArmData::COLLECT);
		telemState = "PRESSED";
	}
//	else if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::HUMAN_LOAD))
//	{
//		m_armData->SetDesiredPosition(CollectorArmData::INTERMEDIATE);
//		telemState = "PRESSED";
//	}
	else if (m_operator_stick->IsButtonJustReleased(DriverStationConfig::JoystickButtons::FIRE_PREPARE)
			|| m_operator_stick->IsButtonJustReleased(DriverStationConfig::JoystickButtons::HUMAN_LOAD)
			|| m_operator_stick->IsButtonJustReleased(DriverStationConfig::JoystickButtons::FIRE_PREPARE))
	{
		m_armData->SetDesiredPosition(CollectorArmData::STOWED);
		telemState = "RELEASED";
	}
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARM_INPUT, telemState);
}
