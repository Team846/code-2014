#include "Pass.h"
#include "../../Config/ConfigPortMappings.h"

Pass::Pass() :
	Automation("Pass", true),
	Configurable("Pass")
{
	m_collectorArm = CollectorArmData::Get();
	m_collectorRollers = CollectorRollersData::Get();
	m_drivetrain = DrivetrainData::Get();
	m_gearTooth = SensorFactory::GetGearTooth(ConfigPortMappings::Get("Digital/COLLECTOR_GEAR_TOOTH"));
}

void Pass::AllocateResources()
{
	AllocateResource(ControlResource::COLLECTOR_ARM);
	AllocateResource(ControlResource::COLLECTOR_ROLLERS);
	AllocateResource(ControlResource::DRIVE);
	AllocateResource(ControlResource::TURN);
}

bool Pass::Start()
{
	m_startTicks = m_gearTooth->Get();
	return true;
}

bool Pass::Run()
{
	if (!Aborting())
	{
		if (m_gearTooth->Get() - m_startTicks >= m_ballReleaseDistance)
		{
			m_collectorArm->SetPosition(CollectorArmData::STOWED);
			m_collectorRollers->SetRunning(false);
			return true;
		}
		else
		{
			m_collectorArm->SetPosition(CollectorArmData::COLLECT);
			m_collectorRollers->SetRunning(true);
			m_collectorRollers->SetDirection(CollectorRollersData::REVERSE);
			m_collectorRollers->SetSpeed(1.0);
			m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
			m_drivetrain->SetControlMode(DrivetrainData::TURN, DrivetrainData::VELOCITY_CONTROL);
			m_drivetrain->SetVelocitySetpoint(DrivetrainData::FORWARD, m_driveBackSpeed);
			m_drivetrain->SetVelocitySetpoint(DrivetrainData::TURN, 0.0);
		}
		return false;
	}
	else
	{
		m_collectorArm->SetPosition(CollectorArmData::STOWED);
		m_collectorRollers->SetRunning(false);
		return true;
	}
}

bool Pass::Abort()
{
	return true;
}

void Pass::Configure()
{
	m_ballReleaseDistance = GetConfig("ball_release_gear_ticks", 0);
	m_driveBackSpeed = GetConfig("drive_back_speed", -1.0);
}
