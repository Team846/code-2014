#include "CollectorArmInputs.h"
#include "../../Config/RobotConfig.h"

CollectorArmInputs::CollectorArmInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	m_armData = CollectorArmData::Get();
	RegisterResource(ControlResource::COLLECTOR_ARM);
}

void CollectorArmInputs::Update()
{
	if (m_operator_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::COLLECTOR_DOWN))
	{
		m_armData->SetDesiredPosition(CollectorArmData::COLLECT);
	}
	else if  (m_operator_stick->IsButtonJustReleased(DriverStationConfig::JoystickButtons::COLLECTOR_DOWN))
	{
		m_armData->SetDesiredPosition(CollectorArmData::STOWED);
	}
}
