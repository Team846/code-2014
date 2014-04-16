#include "PressurePlateInputs.h"
#include "../../Config/RobotConfig.h"
#include "../../Communication/Dashboard2.h"
#include "../../Communication/DashboardTelemetryID.h"

PressurePlateInputs::PressurePlateInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_pressurePlate = PressurePlateData::Get();
	
	Dashboard2::AddTelemetryData("Pressure Plate Input", (INT16)DashboardTelemetryID::PRESSURE_PLATE_INPUT, DashboardTelemetryType::STRING);
	
	RegisterResource(ControlResource::PRESSURE_PLATE);
}

void PressurePlateInputs::Update()
{
	std::string state = "???";
	
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PURGE_LAUNCHER)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::FIRE_PREPARE)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PRESSURE_PLATE)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::HUMAN_LOAD))
	{
		m_pressurePlate->SetPressure(false);
		
		state = "PRESSED";
	}
	else
	{
		m_pressurePlate->SetPressure(true);
		
		state = "RELEASED";
	}
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::PRESSURE_PLATE_INPUT, state);
}
