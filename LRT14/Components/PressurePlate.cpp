#include "PressurePlate.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/Pneumatics.h"

PressurePlate::PressurePlate() :
	Component("PressurePlate", DriverStationConfig::DigitalIns::LAUNCHER_ANGLE),
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
	if (m_pressurePlateData->GetPressure())
	{
		m_pneumatics->Set(Pneumatics::OFF);
	}
	else
	{
		m_pneumatics->Set(Pneumatics::FORWARD);
	}
}

void PressurePlate::UpdateDisabled()
{
	m_pneumatics->Set(Pneumatics::OFF);
}

void PressurePlate::Configure()
{
	
}
