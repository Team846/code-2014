#include "CollectorArmInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"
#include "../../Sensors/DriveEncoders.h"

CollectorArmInputs::CollectorArmInputs() :
	Configurable("CollectorArmInputs")
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_armData = CollectorArmData::Get();
	m_lastSpeed = 0;
	Dashboard2::AddTelemetryData("Collector Arm Input", (INT16)DashboardTelemetryID::COLLECTOR_ARM_INPUT, DashboardTelemetryType::STRING);
	RegisterResource(ControlResource::COLLECTOR_ARM);
}

void CollectorArmInputs::Update()
{
	std::string telemState = "???";

	float accel = DriveEncoders::Get()->GetNormalizedForwardSpeed() - m_lastSpeed;
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::OVERRIDE_FIRE))
	{
		m_armData->SetDesiredPosition(CollectorArmData::COLLECT);
		telemState = "PRESSED";
	}
	else if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::FIRE_PREPARE)
			|| m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::DRIBBLE))
	{
		if (DriveEncoders::Get()->GetNormalizedForwardSpeed() < 0 && accel >= m_hairTriggerAcceleration)
			m_armData->SetDesiredPosition(CollectorArmData::COLLECT);
		else
			m_armData->SetDesiredPosition(CollectorArmData::INTERMEDIATE);
		telemState = "PRESSED";
	}
	else if (m_operator_stick->IsButtonJustReleased(DriverStationConfig::JoystickButtons::FIRE_PREPARE)
			|| m_operator_stick->IsButtonJustReleased(DriverStationConfig::JoystickButtons::OVERRIDE_FIRE))
	{
		m_armData->SetDesiredPosition(CollectorArmData::STOWED);
		telemState = "RELEASED";
	}
	m_lastSpeed = DriveEncoders::Get()->GetNormalizedForwardSpeed();
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARM_INPUT, telemState);
}

void CollectorArmInputs::Configure()
{
	m_hairTriggerAcceleration = GetConfig("hair_trigger_accel", 1);
}
