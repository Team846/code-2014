#include "CollectorArm.h"

#include "../Config/ConfigRuntime.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/RobotConfig.h"
#include "../Utils/Util.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/AsyncCANJaguar.h"
#include "../Actuators/LRTJaguar.h"

CollectorArm::CollectorArm() : 
	Component("CollectorArm", DriverStationConfig::DigitalIns::COLLECTOR_ARM, true),
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

void CollectorArm::Send()
{
	
}
