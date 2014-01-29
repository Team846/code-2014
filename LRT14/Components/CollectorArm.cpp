#include "CollectorArm.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"

CollectorArm::CollectorArm() : 
	Component("CollectorArm", DriverStationConfig::DigitalIns::COLLECTOR_ARM),
	Configurable("CollectorArm")
{
	m_talon = new LRTTalon(ConfigPortMappings::Get("PWM/COLLECTOR_ARM"), "CollectorArm");
	analogChannel = new AnalogChannel(ConfigPortMappings::Get("Analog/COLLECTOR_ARM"));
}

CollectorArm::~CollectorArm()
{
	
}

void CollectorArm::OnEnabled()
{
	
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
}
