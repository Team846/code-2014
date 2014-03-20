#include "PressurePlate.h"

#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/Pneumatics.h"

PressurePlate::PressurePlate() :
	Component("PressurePlate", DriverStationConfig::DigitalIns::PRESSURE_PLATE),
	Configurable("PressurePlate")
{
	m_pressurePlateData = PressurePlateData::Get();
	m_pneumatics = new Pneumatics(ConfigPortMappings::Get("Solenoid/PRESSURE_PLATE"), "PressurePlate");
}

PressurePlate::~PressurePlate()
{

}

void PressurePlate::OnEnabled()
{
	
}
void PressurePlate::OnDisabled()
{
	
}
		
void PressurePlate::UpdateEnabled()
{
	Pneumatics::State state = Pneumatics::OFF;
	std::string stateStr = "OFF";
	
	if (m_pressurePlateData->GetPressure())
	{
		state = Pneumatics::OFF;
		stateStr = "OFF";
	}
	else
	{
		state = Pneumatics::FORWARD;
		stateStr = "FORWARD";
	}
	m_pneumatics->Set(state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::PRESSURE_PLATE_STATE, stateStr);
}

void PressurePlate::UpdateDisabled()
{
	std::string stateStr = "OFF";
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::PRESSURE_PLATE_STATE, stateStr);
}

void PressurePlate::Configure()
{
	
}
