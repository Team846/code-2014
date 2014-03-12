#include "Dribble.h"
#include <Rhesus/Toolkit/Utilities/MathUtils.h>

using namespace Rhesus::Toolkit::Utilities;

Dribble::Dribble():
	Automation("Dribble"),
	Configurable("Dribble")
{
	m_armData = CollectorArmData::Get();
	m_rollerData = CollectorRollersData::Get();
	m_encoders = DriveEncoders::Get();
	m_gearTooth = SensorFactory::GetGearTooth(ConfigPortMappings::Get("Digital/COLLECTOR_GEAR_TOOTH"));
}

void Dribble::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
}

bool Dribble::Start()
{
	return true;
}

bool Dribble::Run()
{
	m_armData->SetDesiredPosition(CollectorArmData::COLLECT);
	m_rollerData->SetRunning(true);
	m_rollerData->SetDirection(CollectorRollersData::FORWARD);
	float setpoint = m_encoders->GetRobotForwardSpeed() / m_rollerMaxSurfaceSpeed * m_extraScale;
	m_rollerData->SetSpeed(-setpoint + m_offset);
	return false;
}

bool Dribble::Abort()
{
	m_rollerData->SetRunning(false);
	m_armData->SetDesiredPosition(CollectorArmData::STOWED);
	return true;
}

void Dribble::Configure()
{
	m_rollerMaxSpeed = GetConfig("max_tick_rate", 40 * 13000.0 / 10.0 / 60);
	m_rollerMaxSurfaceSpeed = m_rollerMaxSpeed / 40 * GetConfig("roller_diameter", 3.875) * acos(-1);
	m_initialSpeed = GetConfig("initial_speed", 0.5);
	m_ballSpeedDropThreshold = GetConfig("ball_speed_drop_threshold", 0.2);
	m_extraScale = GetConfig("extra_speed_scale", 1.05);
	m_offset = GetConfig("speed_offset", 0.1);
}
