#include "CollectorInputs.h"

CollectorInputs::CollectorInputs()
{
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_driver_wheel = LRTDriverStation::Instance()->GetDriverWheel();
	
	collectorData = CollectorData::Get();
	RegisterResource(ControlResource::COLLECTOR_ROLLERS);
}

void CollectorInputs::Update()
{
	collectorData->SetEnabled(false);
	
	if(m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::COLLECT))
	{
		collectorData->SetEnabled(true);
		collectorData->SetDirection(CollectorData::COLLECT);
	}
	else if(m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PURGE))
	{
		collectorData->SetEnabled(true);
		collectorData->SetDirection(CollectorData::PURGE);
	}
}
