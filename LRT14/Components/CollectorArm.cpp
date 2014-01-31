#include "CollectorArm.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"

CollectorArm::CollectorArm() : 
	Component("CollectorArm", DriverStationConfig::DigitalIns::COLLECTOR_ARM),
	Configurable("CollectorArm")
{
	m_armData = CollectorArmData::Get();
	m_pneumatics = new Pneumatics(ConfigPortMappings::Get("Solenoid/COLLECTOR_ARM"), "CollectorArm");
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
	switch(m_armData->GetPosition())
	{
	case CollectorArmData::COLLECT:
		m_pneumatics->Set(Pneumatics::FORWARD);
		break;
	case CollectorArmData::STOWED:
		m_pneumatics->Set(Pneumatics::OFF);
		break;
	default:
		m_pneumatics->Set(Pneumatics::OFF);
	}
}

void CollectorArm::UpdateDisabled()
{
	m_pneumatics->Set(Pneumatics::OFF);	
}

void CollectorArm::Configure()
{
	
}
