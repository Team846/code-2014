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
	m_intermediateLock = new Pneumatics(ConfigPortMappings::Get("Solenoid/COLLECTOR_INTERMEDIATE"), "CollectorIntermediate");
	m_switch = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/COLLECTOR_SWITCH"));
	m_goingToIntermediate = false;
	m_intermediateComplete = false;
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
	std::string stateStr = "???";

	CollectorArmData::Position pos = CollectorArmData::STOWED;

	std::string posStr = "STOWED";
	
	if (m_switch->Get() == 1)
	{
		pos = CollectorArmData::COLLECT;
		posStr = "COLLECT";
	}
	
	m_armData->SetCurrentPosition(pos);

	m_intermediateLock->Set(Pneumatics::OFF);
	
	switch(m_armData->GetDesiredPosition())
	{
	case CollectorArmData::COLLECT:
		state = Pneumatics::FORWARD;
		m_goingToIntermediate = false;
		m_intermediateComplete = false;
		stateStr = "COLLECT";
		break;
	case CollectorArmData::STOWED:
		state = Pneumatics::REVERSE;
		m_goingToIntermediate = false;
		m_intermediateComplete = false;
		stateStr = "STOWED";
		break;
	case CollectorArmData::INTERMEDIATE:
		if (!m_goingToIntermediate)
		{
			m_goingToIntermediate = true;
			m_intermediateStartingPosition = pos;
		}
		state = m_intermediateStartingPosition == CollectorArmData::COLLECT ? Pneumatics::REVERSE : Pneumatics::FORWARD;
		if (pos != m_intermediateStartingPosition && m_goingToIntermediate)
		{
			m_intermediateComplete = true;
		}
		if (m_intermediateComplete)
		{
			m_intermediateLock->Set(Pneumatics::FORWARD);
			state = pos == CollectorArmData::STOWED ? Pneumatics::FORWARD : Pneumatics::REVERSE;
		}
		stateStr = "INTERMEDIATE";
		break;
	}
	
	m_pneumatics->Set(state);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_DESIRED_POSITION, stateStr);
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_CURRENT_POSITION, posStr);
}

void CollectorArm::UpdateDisabled()
{
	CollectorArmData::Position pos = CollectorArmData::STOWED;
	std::string posStr = "STOWED";
	
	if (m_switch->Get() == 1)
	{
		pos = CollectorArmData::COLLECT;
		posStr = "COLLECT";
	}
	m_intermediateLock->Set(Pneumatics::OFF);
	m_goingToIntermediate = false;
	m_intermediateComplete = false;
	
	m_armData->SetCurrentPosition(pos);
	
	std::string desiredPosStr = "???";
	
	switch(m_armData->GetDesiredPosition())
	{
	case CollectorArmData::COLLECT:
		desiredPosStr = "COLLECT";
		break;
	case CollectorArmData::STOWED:
		desiredPosStr = "STOWED";
		break;
	case CollectorArmData::INTERMEDIATE:
		desiredPosStr = "INTERMEDIATE";
		break;
	}
	
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_DESIRED_POSITION, desiredPosStr);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ARMS_CURRENT_POSITION, posStr);
}

void CollectorArm::Configure()
{
	
}

void CollectorArm::Send()
{
	SendToNetwork(m_switch->Get(), "CollectorSwitch", "RobotData");
}
