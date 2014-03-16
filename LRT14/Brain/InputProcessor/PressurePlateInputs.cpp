#include "PressurePlateInputs.h"
#include "../../Config/RobotConfig.h"

PressurePlateInputs::PressurePlateInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_pressurePlate = PressurePlateData::Get();
	RegisterResource(ControlResource::PRESSURE_PLATE);
}

void PressurePlateInputs::Update()
{
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PURGE_LAUNCHER)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::COLLECTOR_DOWN)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PRESSURE_PLATE))
	{
		m_pressurePlate->SetPressure(false);
	}
	else
	{
		m_pressurePlate->SetPressure(true);
	}
}
