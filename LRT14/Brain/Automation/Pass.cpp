#include "Pass.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../DriverStation/LRTDriverStation.h"
#include <Rhesus/Toolkit/Utilities/MathUtils.h>
#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;

using namespace Rhesus::Toolkit::Utilities;

Pass::Pass() :
	Automation("Pass"),
	Configurable("Pass")
{
	m_collectorArm = CollectorArmData::Get();
	m_rollersData = CollectorRollersData::Get();
	m_pressurePlate = PressurePlateData::Get();
	m_gearTooth = SensorFactory::GetGearTooth(ConfigPortMappings::Get("Digital/COLLECTOR_GEAR_TOOTH"));
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
	m_ballPassed = false;
	m_passingToGround = false;
}

void Pass::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::PRESSURE_PLATE);
}

bool Pass::Start()
{
	m_ballPassed = false;
	m_passingToGround = false;
	m_startTicks = m_gearTooth->Get();
	return true;
}

bool Pass::Run()
{
	m_pressurePlate->SetPressure(false);
	m_restSpeed = MathUtils::Clamp(-LRTDriverStation::Instance()->GetOperatorStick()->GetAxis(Joystick::kYAxis), (float)0.0, (float)1.0);
	float driveSpeed = DriveEncoders::Get()->GetRobotForwardSpeed() / m_rollerMaxSurfaceSpeed;
	if (driveSpeed < 0)
	{
		m_restSpeed += -driveSpeed;
	}
	printf("%f\n", 1 / m_gearTooth->GetPeriod());
	if (m_gearTooth->Get() - m_startTicks >= m_ballReleaseDistance)
	{
		m_rollersData->SetSpeed(m_restSpeed);
		float speed = (m_gearTooth->GetStopped() ? 0.0 : 1 / m_gearTooth->GetPeriod()) / m_rollerMaxSpeed;
		if (fabs(speed - m_restSpeed) <= m_threshold)
		{
			m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
			m_pressurePlate->SetPressure(true);
			if (m_collectorArm->GetCurrentPosition() == CollectorArmData::STOWED)
			{
				m_rollersData->SetRunning(false);
				return true;
			}
		}
	}
	else
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
		m_rollersData->SetRunning(true);
		m_rollersData->SetDirection(CollectorRollersData::REVERSE);
		m_rollersData->SetSpeed(MathUtils::Clamp((m_ballReleaseDistance - (m_gearTooth->Get() - m_startTicks)) * m_gain, m_restSpeed, m_rollerSpeed));
	}
	return false;
}

bool Pass::Abort()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_rollersData->SetRunning(false);
	m_pressurePlate->SetPressure(true);
	return true;
}

void Pass::Configure()
{
	m_ballReleaseDistance = GetConfig("ball_release_gear_ticks", 0);
	m_rollerSpeed = GetConfig("roller_speed", 1.0);
	m_gain = GetConfig("gain", 0.1);
	m_rollerMaxSpeed = GetConfig("max_tick_rate", 40 * 13000.0 / 10.0 / 60);
	m_rollerMaxSurfaceSpeed = m_rollerMaxSpeed  / 40 * GetConfig("roller_diameter", 3.875) * acos(-1);
	m_threshold = GetConfig("speed_drop_threshold", 0.05);
}
