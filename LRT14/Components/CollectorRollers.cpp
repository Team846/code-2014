#include "CollectorRollers.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"

CollectorRollers::CollectorRollers() : 
	Component("CollectorRollers", DriverStationConfig::DigitalIns::COLLECTOR_ROLLERS),
	Configurable("CollectorRollers")
{
	m_rollersData = CollectorRollersData::Get();
	m_motor = new LRTTalon(ConfigPortMappings::Get("PWM/COLLECTOR_ROLLERS"), "CollectorRollers");
}

CollectorRollers::~CollectorRollers()
{
	R_DELETE(m_motor);
}

void CollectorRollers::OnEnabled()
{
	
}

void CollectorRollers::OnDisabled()
{
	
}

void CollectorRollers::UpdateEnabled()
{
	if (m_rollersData->IsRunning())
	{
		if (m_rollersData->GetDirection() == CollectorRollersData::FORWARD)
			m_motor->SetDutyCycle(m_rollersData->GetSpeed() * m_forwardSpeed);
		if (m_rollersData->GetDirection() == CollectorRollersData::REVERSE)
			m_motor->SetDutyCycle(m_rollersData->GetSpeed() * m_reverseSpeed);
	}
	else
		m_motor->SetDutyCycle(0.0);
}

void CollectorRollers::UpdateDisabled()
{
	m_motor->SetDutyCycle(0.0);
}

void CollectorRollers::Configure()
{
	m_forwardSpeed = GetConfig("forward_speed", 1.0);
	m_reverseSpeed = GetConfig("reverse_speed", -1.0);
}
