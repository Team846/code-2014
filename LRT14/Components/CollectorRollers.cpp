#include "CollectorRollers.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"

CollectorRollers::CollectorRollers() : 
	Component("CollectorRollers", DriverStationConfig::DigitalIns::COLLECTOR_ROLLERS),
	Configurable("CollectorRollers")
{
	
}

CollectorRollers::~CollectorRollers()
{
	
}

void CollectorRollers::OnEnabled()
{
	
}

void CollectorRollers::OnDisabled()
{
	
}

void CollectorRollers::UpdateEnabled()
{
	
}

void CollectorRollers::UpdateDisabled()
{
	
}

void CollectorRollers::Configure()
{
	
}
