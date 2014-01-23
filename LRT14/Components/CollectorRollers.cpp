#include "CollectorRollers.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"

CollectorRollers::CollectorRollers() : 
	Component("CollectorRollers", DriverStationConfig::DigitalIns::COLLECTOR_ROLLERS),
	Configurable("CollectorRollers")
{
	m_rollerData = CollectorRollerData::Get();
	m_talon = new LRTTalon(ConfigPortMappings::Get("PWM/COLLECTOR_ROLLERS"));
}

CollectorRollers::~CollectorRollers()
{
	
}

void CollectorRollers::OnEnabled()
{
	
}

void CollectorRollers::OnDisabled()
{
	
}

void CollectorRollers::UpdateEnabled()
{
	if(m_rollerData->IsRunning())
	{
		if(m_rollerData->GetDirection() == CollectorRollerData::FORWARD)
			m_talon->SetDutyCycle(m_forwardSpeed);
		else
			m_talon->SetDutyCycle(-1.0);
	}
	else
		m_talon->SetDutyCycle(m_reverseSpeed);
}

void CollectorRollers::UpdateDisabled()
{
	m_talon->SetDutyCycle(0.0);
}

void CollectorRollers::Configure()
{
	m_forwardSpeed = GetConfig("forward_speed", 1.0);
	m_reverseSpeed = GetConfig("reverse_speed", -1.0);
}
