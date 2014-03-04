#include "Pass.h"
#include "../../Config/ConfigPortMappings.h"

Pass::Pass() :
	Automation("Pass"),
	Configurable("Pass")
{
	m_collectorArm = CollectorArmData::Get();
	m_loaderData = CollectorRollersData::Get();
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
	if (m_gearTooth->Get() - m_startTicks >= m_ballReleaseDistance)
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
		m_loaderData->SetRunning(false);
		return true;
	}
	else
	{
		m_collectorArm->SetDesiredPosition(CollectorArmData::COLLECT);
		m_loaderData->SetRunning(true);
		m_loaderData->SetDirection(CollectorRollersData::REVERSE);
		m_loaderData->SetSpeed(1.0);
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
		m_drivetrain->SetControlMode(DrivetrainData::TURN, DrivetrainData::VELOCITY_CONTROL);
		m_drivetrain->SetVelocitySetpoint(DrivetrainData::FORWARD, m_driveBackSpeed);
		m_drivetrain->SetVelocitySetpoint(DrivetrainData::TURN, 0.0);
	}
	return false;
}

bool Pass::Abort()
{
	m_collectorArm->SetDesiredPosition(CollectorArmData::STOWED);
	m_loaderData->SetRunning(false);
	return true;
}

void Pass::Configure()
{
	m_ballReleaseDistance = GetConfig("ball_release_gear_ticks", 0);
	m_driveBackSpeed = GetConfig("drive_back_speed", -1.0);
}
