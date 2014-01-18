#include "CollectorRollers.h"

#include "../Config/ConfigRuntime.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/RobotConfig.h"
#include "../Utils/Util.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/AsyncCANJaguar.h"
#include "../Actuators/LRTJaguar.h"

CollectorRollers::CollectorRollers() : 
	Component("CollectorRollers", DriverStationConfig::DigitalIns::COLLECTOR_ROLLERS, true),
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

void CollectorRollers::Send()
{
	
}
