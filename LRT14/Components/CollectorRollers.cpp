#include "CollectorRollers.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/LRTTalon.h"
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
	if (m_rollersData->IsRunning())
	{
		if (m_rollersData->GetDirection() == CollectorRollersData::FORWARD)
			m_motor->SetDutyCycle(m_rollersData->GetSpeed() * m_forwardSpeed);
		else if (m_rollersData->GetDirection() == CollectorRollersData::REVERSE)
			m_motor->SetDutyCycle(m_rollersData->GetSpeed() * m_reverseSpeed);
	}
	else
		m_motor->SetDutyCycle(0.0);
}

void CollectorRollers::UpdateDisabled()
{
	m_motor->SetDutyCycle(0.0);
}

void CollectorRollers::Configure()
{
	m_forwardSpeed = GetConfig("forward_speed", 1.0);
	m_reverseSpeed = GetConfig("reverse_speed", -1.0);
}

void CollectorRollers::Send()
{
	SendToNetwork(m_gearTooth->Get(), "GearToothTicks", "RobotData");
	SendToNetwork(m_gearTooth->GetDirection() ? 1 : -1, "GearToothDirection", "RobotData");
}
