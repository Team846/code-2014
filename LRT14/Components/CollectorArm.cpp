#include "CollectorArm.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"

CollectorArm::CollectorArm() : 
	Component("CollectorArm", DriverStationConfig::DigitalIns::COLLECTOR_ARM),
	Configurable("CollectorArm")
{
	m_armData = CollectorArmData::Get();
	m_talon = new LRTTalon(ConfigPortMappings::Get("PWM/COLLECTOR_ARM"), "CollectorArm");
	m_pot = new AnalogChannel(ConfigPortMappings::Get("Analog/COLLECTOR_ARM"));
}

CollectorArm::~CollectorArm()
{
	DELETE(m_talon);
	DELETE(m_pot);
}

void CollectorArm::OnEnabled()
{
	switch (m_armData->GetPosition())
	{
	case CollectorArmData::STOWED:
		m_talon->SetDutyCycle(ComputeOutput(m_stowedSetpoint));
		break;
	case CollectorArmData::COLLECT:
		m_talon->SetDutyCycle(ComputeOutput(m_collectSetpoint));
		break;
	default:
		m_talon->SetDutyCycle(0.0);
	}
}

float CollectorArm::ComputeOutput(int setpoint)
{
	float error = (float)(setpoint - m_pot->GetAverageValue()) / (float)m_maxPotValue;
	int sign = (error > 0 ? 1 : -1);
	return sign * pow(fabs(error), 1.0/3.0);
}

void CollectorArm::OnDisabled()
{
	
}

void CollectorArm::UpdateEnabled()
{
	
}

void CollectorArm::UpdateDisabled()
{
	
}

void CollectorArm::Configure()
{
	m_collectSetpoint = GetConfig("collect_setpoint", 0);
	m_stowedSetpoint = GetConfig("stowed_setpoint", 0);
	m_maxPotValue = GetConfig("max_pot_value", 0);
}
