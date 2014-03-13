#include "Turn.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Sensors/DriveEncoders.h"

Turn::Turn(double angle, double maxSpeed, double errorThreshold) :
	Automation("Turn")
{
	m_angle = angle;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_drivetrain = DrivetrainData::Get();
}

Turn::Turn() :
	Automation("Turn")
{
	m_angle = 0;
	m_maxSpeed = 0;
	m_errorThreshold = 0;
	
	m_drivetrain = DrivetrainData::Get();
}

void Turn::setAngle(double angle)
{
	m_angle = angle;
}

void Turn::setMaxSpeed(double speed)
{
	m_maxSpeed = speed;
}

void Turn::setErrorThreshold(double error)
{
	m_errorThreshold = error;
}

void Turn::AllocateResources()
{
	AllocateResource(ControlResource::TURN);
}

bool Turn::Start()
{
	m_drivetrain->SetControlMode(DrivetrainData::TURN, DrivetrainData::POSITION_CONTROL);
	m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::TURN, m_angle);
	m_drivetrain->SetPositionControlMaxSpeed(DrivetrainData::TURN, m_maxSpeed);
	return true;
}

bool Turn::Run()
{
	return fabs(DriveEncoders::Get()->GetTurnAngle() - m_drivetrain->GetPositionSetpoint(DrivetrainData::TURN)) < m_errorThreshold;
}

bool Turn::Abort()
{
	return true;
}
