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
	float setpoint = m_encoders->GetRobotForwardSpeed() / m_rollerMaxSpeed;
	float speed = MathUtils::Sign(speed) / (m_gearTooth->GetPeriod());
	float error = setpoint - speed * m_ticksToSurface / m_rollerMaxSpeed;
	m_rollerData->SetDirection(error > 0 ? CollectorRollersData::REVERSE : CollectorRollersData::FORWARD);
	m_rollerData->SetSpeed(MathUtils::Abs<float>(setpoint + error * m_gain));
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
	m_rollerMaxSpeed = GetConfig("max_surface_speed", 13000.0 / 10.0 / 60 * 3.875 * acos(-1));
	m_ticksToSurface = GetConfig("ticks_to_surface", (1 / 40.0) * (3.875 * acos(-1)));
	m_gain = GetConfig("m_gain", 1.0);
}
