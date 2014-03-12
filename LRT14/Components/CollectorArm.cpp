#include "CollectorArm.h"

#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
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
	Pneumatics::State state = Pneumatics::REVERSE;
	
	switch(m_armData->GetDesiredPosition())
	{
	case CollectorArmData::COLLECT:
		state = Pneumatics::FORWARD;
		break;
	case CollectorArmData::STOWED:
		state = Pneumatics::REVERSE;
		break;
	}
	
	m_pneumatics->Set(state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_DESIRED_POSITION, (INT8)state);
	
	CollectorArmData::Position pos = CollectorArmData::STOWED;

	if (m_switch->Get() == 0)
	{
		pos = CollectorArmData::COLLECT;
	}
	
	m_armData->SetCurrentPosition(pos);
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_CURRENT_POSITION, (INT8)pos);
}

void CollectorArm::UpdateDisabled()
{
	Pneumatics::State state = Pneumatics::REVERSE;
	
	CollectorArmData::Position pos = CollectorArmData::STOWED;
	
	if (m_switch->Get() == 0)
	{
		pos = CollectorArmData::COLLECT;
	}
	
	m_armData->SetCurrentPosition(pos);
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_DESIRED_POSITION, (INT8)state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_CURRENT_POSITION, (INT8)pos);
}

void CollectorArm::Configure()
{
	
}

void CollectorArm::Send()
{
	SendToNetwork(m_switch->Get(), "CollectorSwitch", "RobotData");
}
