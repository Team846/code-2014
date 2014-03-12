#include "SelfCatch.h"
#include "../../Sensors/DriveEncoders.h"
#include "../../Config/ConfigPortMappings.h"
#include <math.h>

SelfCatch::SelfCatch() :
	Automation("SelfCatch"),
	Configurable("SelfCatch")
{
	m_collectorArm = CollectorArmData::Get();
	m_launcherAngle = LauncherAngleData::Get();
	m_drive = NULL;
	m_turn = new Turn(0.0);
	m_fire = new Fire();
	m_fired = false;
	m_ultrasonic = SensorFactory::GetAnalogChannel(ConfigPortMappings::Get("Analog/CATCH_ULTRASONIC"));
}

void SelfCatch::AllocateResources()
{
	AllocateResource(ControlResource::LAUNCHER_LOADER);
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::PRESSURE_PLATE);
}

bool SelfCatch::Start()
{
	m_fired = false;
	delete m_drive;
	m_fire->StartAutomation(GetStartEvent());
	return true;
}

bool SelfCatch::Run()
{
	m_launcherAngle->SetAngle(LauncherAngleData::SHORT);
	if (!m_fired)
	{
		m_fired = m_fire->Update();
		if (m_fired)
		{
			AllocateResource(ControlResource::DRIVE);
			AllocateResource(ControlResource::TURN);
			float vx = m_launchVelocity * 12 * cos(m_launchAngle * acos(-1) / 180) + DriveEncoders::Get()->GetRobotForwardSpeed();
			float vy = m_launchVelocity * 12 * sin(m_launchAngle * acos(-1) / 180);
			float theta = atan2(vy, vx); // Radians
			float v = sqrt(vy * vy + vx * vx);
			float distance = v * v / 386.09 * sin(2 * theta); // Inches
			m_drive = new Drive(distance);
			m_drive->StartAutomation(GetStartEvent());
			m_turn->StartAutomation(GetStartEvent());
		}
		else
		{
			return false;
		}
	}
	m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
	m_drive->Update();
	m_turn->Update();
	if (m_ultrasonic->GetAverageValue() <= m_ballEnterDistance)
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
		return true;
	}
	return false;
}

bool SelfCatch::Abort()
{
	if (!m_fired)
	{
		m_fire->AbortAutomation(GetAbortEvent());
	}
	else
	{
		m_drive->AbortAutomation(GetAbortEvent());
		m_turn->AbortAutomation(GetAbortEvent());
	}
	m_fired = false;
	return true;
}

void SelfCatch::Configure()
{
	m_ballEnterDistance = GetConfig("ball_enter_distance", 0);
	m_launchVelocity = GetConfig("launch_velocity", 20.94); // Feet per second
	m_launchAngle = GetConfig("launch_angle", 72.02);
}
