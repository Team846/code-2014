#include "Pass.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../DriverStation/LRTDriverStation.h"
#include <Rhesus/Toolkit/Utilities/MathUtils.h>

using namespace Rhesus::Toolkit::Utilities;

Pass::Pass() :
	Automation("Pass"),
	Configurable("Pass")
{
	m_collectorArm = CollectorArmData::Get();
	m_rollersData = CollectorRollersData::Get();
	m_gearTooth = SensorFactory::GetGearTooth(ConfigPortMappings::Get("Digital/COLLECTOR_GEAR_TOOTH"));
	m_proximity = SensorFactory::GetDigitalInput(ConfigPortMappings::Get("Digital/BALL_BUMPER_PROXIMITY"));
	m_ballPassed = false;
	m_passingToGround = false;
}

void Pass::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
}

bool Pass::Start()
{
	m_ballPassed = false;
	m_passingToGround = false;
	m_startTicks = m_gearTooth->Get();
	m_restSpeed = MathUtils::Clamp(-LRTDriverStation::Instance()->GetOperatorStick()->GetAxis(Joystick::kYAxis), (float)0.0, (float)1.0);
	return true;
}

bool Pass::Run()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
	m_rollersData->SetRunning(true);
	m_rollersData->SetDirection(CollectorRollersData::REVERSE);
	m_rollersData->SetSpeed(m_rollerSpeed);
//	if (m_proximity->Get() == 1)
//	{
//		m_ballPassed = true;
//	}
//	else if (m_proximity->Get() == 0 && m_ballPassed && !m_passingToGround)
//	{
//		m_passingToGround = true;
//		m_startTicks = m_gearTooth->Get();
//		m_restSpeed = MathUtils::Clamp(-LRTDriverStation::Instance()->GetOperatorStick()->GetAxis(Joystick::kYAxis), (float)0.0, (float)1.0);
//	}
//	if (m_passingToGround)
//	{
		if (m_gearTooth->Get() - m_startTicks >= m_ballReleaseDistance)
		{
			m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
			m_rollersData->SetSpeed(m_restSpeed);
			if (m_collectorArm->GetCurrentPosition() == CollectorArmData::STOWED)
			{
				m_rollersData->SetRunning(false);
				return true;
			}
		}
//	}
	return false;
}

bool Pass::Abort()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_rollersData->SetRunning(false);
	return true;
}

void Pass::Configure()
{
	m_ballReleaseDistance = GetConfig("ball_release_gear_ticks", 0);
	m_rollerSpeed = GetConfig("roller_speed", 1.0);
}
