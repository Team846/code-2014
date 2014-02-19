#include "CollectorArm.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/Pneumatics.h"
#include "../Sensors/SensorFactory.h"

CollectorArm::CollectorArm() : 
	Component("CollectorArm", DriverStationConfig::DigitalIns::COLLECTOR_ARM),
	Configurable("CollectorArm")
{
	m_armData = CollectorArmData::Get();
	m_pneumatics = new Pneumatics(ConfigPortMappings::Get("Solenoid/COLLECTOR_ARM_A"), ConfigPortMappings::Get("Solenoid/COLLECTOR_ARM_B"), "CollectorArm");
	m_switch = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/COLLECTOR_SWITCH"));
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
	switch(m_armData->GetDesiredPosition())
	{
	case CollectorArmData::COLLECT:
		m_pneumatics->Set(Pneumatics::FORWARD);
		break;
	case CollectorArmData::STOWED:
		m_pneumatics->Set(Pneumatics::REVERSE);
		break;
	default:
		m_pneumatics->Set(Pneumatics::REVERSE);
		break;
	}
	
	if (m_switch->Get() == 0)
	{
		m_armData->SetCurrentPosition(CollectorArmData::COLLECT);
	}
	else
	{
		m_armData->SetCurrentPosition(CollectorArmData::STOWED);
	}
}

void CollectorArm::UpdateDisabled()
{
}

void CollectorArm::Configure()
{
	
}
