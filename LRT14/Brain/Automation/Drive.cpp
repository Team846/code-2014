#include "Drive.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Sensors/DriveEncoders.h"

#include <Rhesus.Toolkit.Utilities.h>
#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::Utilities;
using namespace Rhesus::Toolkit::IO;

Drive::Drive(double distance, double maxSpeed, double errorThreshold, bool continuous) :
	Automation("Drive")
{
	m_distance = distance;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_continuous = continuous;
	m_drivetrain = DrivetrainData::Get();
}

void Drive::AllocateResources()
{
	AllocateResource(ControlResource::DRIVE);
}

bool Drive::Start()
{
	if (!m_continuous)
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
	}
	else
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
		m_drivetrain->SetVelocitySetpoint(DrivetrainData::FORWARD, MathUtils::Sign(m_distance) * m_maxSpeed);
	}
	m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::FORWARD, m_distance);
	m_drivetrain->SetPositionControlMaxSpeed(DrivetrainData::FORWARD, m_maxSpeed);
	return true;
}

bool Drive::Run()
{
	float robotLocation = DriveEncoders::Get()->GetRobotDist();
	float setpoint = m_drivetrain->GetPositionSetpoint(DrivetrainData::FORWARD);
	float distanceLeft = setpoint - robotLocation;
	if (m_distance < 0)
		distanceLeft = -distanceLeft; // Going backwards
	
	printf("Distance: %f\n", distanceLeft);
	
	return distanceLeft < m_errorThreshold;
	
//	if (m_distance > 0)
//		return DriveEncoders::Get()->GetRobotDist() > m_drivetrain->GetPositionSetpoint(DrivetrainData::FORWARD);
//	else
//		return DriveEncoders::Get()->GetRobotDist() < m_drivetrain->GetPositionSetpoint(DrivetrainData::FORWARD);
}

bool Drive::Abort()
{
	if (m_continuous)
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
	}
	return true;
}
