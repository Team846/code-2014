#include "CollectorArm.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"

CollectorArm::CollectorArm() : 
	Component("CollectorArm", DriverStationConfig::DigitalIns::COLLECTOR_ARM),
	Configurable("CollectorArm")
{
	
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
	
}
