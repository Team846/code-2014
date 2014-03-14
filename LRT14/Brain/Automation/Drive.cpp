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
	return fabs(DriveEncoders::Get()->GetRobotDist() - m_drivetrain->GetPositionSetpoint(DrivetrainData::FORWARD)) < m_errorThreshold;
}

bool Drive::Abort()
{
	if (m_continuous)
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
	}
	return true;
}
