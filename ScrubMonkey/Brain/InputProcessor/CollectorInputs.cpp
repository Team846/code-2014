#include "CollectorInputs.h"

#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::IO;

CollectorInputs::CollectorInputs()
{
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	
	collectorData = CollectorData::Get();
	indicatorData = IndicatorData::Get();
	RegisterResource(ControlResource::COLLECTOR_ROLLERS);
}

void CollectorInputs::Update()
{	
	collectorData->SetEnabled(false);
	
	if(m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::COLLECT))
	{
		collectorData->SetEnabled(true);
		collectorData->SetDirection(CollectorData::COLLECT);
		indicatorData->SetShooterPattern(IndicatorData::LOADED);
		
	}
	else if(m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PURGE))
	{
		collectorData->SetEnabled(true);
		collectorData->SetDirection(CollectorData::PURGE);
	}
}
