#include "Drive.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Sensors/DriveEncoders.h"
#include <Timer.h>

#include <Rhesus.Toolkit.Utilities.h>
#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::Utilities;
using namespace Rhesus::Toolkit::IO;

Drive::Drive(double distance, double maxSpeed, double errorThreshold, bool continuous) :
	Automation("Drive"),
	Configurable("DrivePosition")
{
	m_distance = distance;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_continuous = continuous;
	m_drivetrain = DrivetrainData::Get();
	
	Configure();
	m_profile = new TrapezoidProfile(maxSpeed * m_maxVelocity, m_timeToMax);
}

void Drive::AllocateResources()
{
	AllocateResource(ControlResource::DRIVE);
}

bool Drive::Start()
{
	if (!m_continuous)
	{
		m_profile->UpdateValues(m_maxSpeed * m_maxVelocity, m_timeToMax);
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
		m_start = m_drivetrain->GetPositionSetpoint(DrivetrainData::FORWARD);
		m_profile->SetSetpoint(m_distance, Timer::GetFPGATimestamp());
		m_drivetrain->SetVelocitySetpoint(DrivetrainData::FORWARD, 0.0);
	}
	else
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
		m_drivetrain->SetVelocitySetpoint(DrivetrainData::FORWARD, MathUtils::Sign(m_distance) * m_maxSpeed);
		m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::FORWARD, m_distance);
	}
	m_drivetrain->SetPositionControlMaxSpeed(DrivetrainData::FORWARD, m_maxSpeed);
	return true;
}

bool Drive::Run()
{
	if (!m_continuous)
	{
		m_drivetrain->SetPositionSetpoint(DrivetrainData::FORWARD, m_start + m_profile->Update(Timer::GetFPGATimestamp()));
		BufferedConsole::Printf("%f %f %f %f\n", m_profile->GetVelocity(), m_profile->GetOutput(), DriveEncoders::Get()->GetRobotDist() - m_start);
		m_drivetrain->SetVelocitySetpoint(DrivetrainData::FORWARD, m_profile->GetVelocity() / m_maxVelocity);
	}
	if (fabs(DriveEncoders::Get()->GetRobotDist() - (m_start + m_distance)) < m_errorThreshold)
	{
		m_drivetrain->SetPositionSetpoint(DrivetrainData::FORWARD, m_start + m_distance);
		return true;
	}
	return false;
}

bool Drive::Abort()
{
	if (m_continuous)
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
	}
	m_drivetrain->SetPositionSetpoint(DrivetrainData::FORWARD, m_start + m_distance);
	return true;
}

void Drive::Configure()
{
	m_maxVelocity = GetConfig("max_speed", 192.0);
	m_timeToMax = GetConfig("time_to_max", 1.0);
}
