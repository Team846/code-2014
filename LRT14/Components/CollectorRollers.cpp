#include "CollectorRollers.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"
#include "../Communication/Dashboard2.h"
#include "../Communication/DashboardTelemetryID.h"
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;

CollectorRollers::CollectorRollers() : 
	Component("CollectorRollers", DriverStationConfig::DigitalIns::COLLECTOR_ROLLERS),
	Configurable("CollectorRollers")
{
	m_rollersData = CollectorRollersData::Get();
	m_motor = new LRTTalon(ConfigPortMappings::Get("PWM/COLLECTOR_ROLLERS"), "CollectorRollers");
	m_gearTooth = SensorFactory::GetGearTooth(ConfigPortMappings::Get("Digital/COLLECTOR_GEAR_TOOTH"));
}

CollectorRollers::~CollectorRollers()
{
	R_DELETE(m_motor);
}

void CollectorRollers::OnEnabled()
{
	
}

void CollectorRollers::OnDisabled()
{
	
}

void CollectorRollers::UpdateEnabled()
{
	double speed = 0.0;
	
	if (m_rollersData->IsRunning())
	{
		if (m_rollersData->GetDirection() == CollectorRollersData::FORWARD)
			speed = m_rollersData->GetSpeed();
		else if (m_rollersData->GetDirection() == CollectorRollersData::REVERSE)
			speed = -m_rollersData->GetSpeed();
	}
	else
	{
		speed = 0;
	}
	
	m_motor->SetDutyCycle(speed);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ROLLERS_SPEED, (float)speed);
}

void CollectorRollers::UpdateDisabled()
{
	m_motor->SetDutyCycle(0.0);
	Dashboard2::SetTelemetryData((INT16)DashboardTelemetryID::COLLECTOR_ROLLERS_SPEED, 0.0f);
}

void CollectorRollers::Configure()
{
}

void CollectorRollers::Send()
{
	SendToNetwork(m_gearTooth->Get(), "GearToothTicks", "RobotData");
	SendToNetwork(m_gearTooth->GetDirection() ? 1 : -1, "GearToothDirection", "RobotData");
}
