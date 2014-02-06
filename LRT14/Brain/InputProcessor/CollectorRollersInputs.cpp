#include "CollectorRollersInputs.h"
#include "../../Config/RobotConfig.h"

CollectorRollersInputs::CollectorRollersInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_rollersData = CollectorRollersData::Get();
	RegisterResource(ControlResource::COLLECTOR_ROLLERS);
}

void CollectorRollersInputs::Update()
{
	m_rollersData->SetRunning(false);
	if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::LOAD_LAUNCHER))
	{
		m_rollersData->SetRunning(true);
		m_rollersData->SetSpeed(1.0);
		m_rollersData->SetDirection(CollectorRollersData::FORWARD);
	}
	else if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::REVERSE_ROLLERS))
	{
		m_rollersData->SetRunning(true);
		m_rollersData->SetSpeed(1.0);
		m_rollersData->SetDirection(CollectorRollersData::REVERSE);
	}
}
