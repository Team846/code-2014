#include "Turn.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Utils/Util.h"
#include "../../Sensors/DriveEncoders.h"

Turn::Turn(double angle, double maxSpeed, double errorThreshold) :
	Automation("Turn")
{
	m_angle = angle;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_drivetrain = DrivetrainData::Get();
}

void Turn::AllocateResources()
{
	AllocateResource(TURN);
}

bool Turn::Start()
{
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
